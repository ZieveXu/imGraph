#include "Block.h"
#include <vector>
#ifdef _WIN32
#pragma warning(disable:4503)
#pragma warning(push)
#pragma warning(disable:4996 4251 4275 4800)
#endif
#include <boost/thread/thread.hpp>
#include <boost/parameter.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/lock_guard.hpp>
#ifdef _WIN32
#pragma warning(pop)
#endif
#include "Graph.h"
#include "blocks/ParamValidator.h"
#include "ProcessManager.h"

using namespace lsis_org;
using std::string;
using std::map;
using std::vector;
using boost::property_tree::ptree;
using boost::lexical_cast;

namespace charliesoft
{
  std::string typeName(ParamType type)
  {
    switch (type)
    {
    case Boolean:
      return "Boolean";
    case Int:
      return "Int";
    case Float:
      return "Float";
    case Color:
      return "Color";
    case Matrix:
      return "Mat";
    case String:
      return "String";
    case FilePath:
      return "FilePath";
    default:
      return "typeError";
    }
  }

  boost::mutex _mtx_synchro;

  ConditionOfRendering::ConditionOfRendering()
  {
    category_left = category_right = boolean_operator = 0;
    _father = 0;
  }
  ConditionOfRendering::ConditionOfRendering(unsigned char category_l,
    ParamValue opt_value_l,
    unsigned char category_r, ParamValue opt_value_r,
    unsigned char boolean_op, Block* father) :
    category_left(category_l), opt_value_left(opt_value_l), category_right(category_r),
    opt_value_right(opt_value_r), boolean_operator(boolean_op), _father(father)
  {
    opt_value_left.setBlock(father);
    opt_value_right.setBlock(father);
  }

  boost::property_tree::ptree ConditionOfRendering::getXML() const
  {
    ptree tree;
    tree.put("category_left", category_left);
    tree.put("category_right", category_right);
    tree.put("boolean_operator", boolean_operator);

    if (!opt_value_left.isLinked())
      tree.put("Value_left", opt_value_left.toString());
    else
      tree.put("Value_left", (unsigned int)opt_value_left.get<ParamValue*>(false));
    if (!opt_value_right.isLinked())
      tree.put("Value_right", opt_value_right.toString());
    else
      tree.put("Value_right", (unsigned int)opt_value_right.get<ParamValue*>(false));
    return tree;
  }


  std::string ConditionOfRendering::toString()
  {
    if (category_left > 3 || category_right > 3 || boolean_operator > 5 || category_left == 0 || category_right == 0)
      return "";
    string left, right;
    switch (category_left)
    {
    case 1://Output of block
      left = "XXX";
      break;
    case 3://cardinal of block rendering
      left = _STR("CONDITION_CARDINAL");
      break;
    default://nothing
      left = opt_value_left.toString();
      break;
    }
    switch (category_right)
    {
    case 1://Output of block
      right = "XXX";
      break;
    case 3://Is empty
      right = _STR("CONDITION_IS_EMPTY");
    default://nothing
      right = opt_value_right.toString();
      break;
    }

    switch (boolean_operator)
    {
    case 0://==
      return left + " == " + right;
    case 1://!=
      return left + " != " + right;
    case 2://<
      return left + " < " + right;
    case 3://>
      return left + " > " + right;
    case 4://<=
      return left + " <= " + right;
    case 5://>=
      return left + " >= " + right;
    default://nothing
      break;
    }

    return "";
  }

  bool ConditionOfRendering::canRender(Block* blockTested)
  {
    if (category_left > 3 || category_right > 3 || boolean_operator>5 || category_left == 0 || category_right == 0)
      return true;
    ParamValue left, right;
    switch (category_left)
    {
    case 1://Output of block
    {
      ParamValue* tmp = opt_value_left.get<ParamValue*>(false);
      if (tmp != NULL)
        left = *tmp;
      break;
    }
    case 2://Constante value
      left = opt_value_left;
      break;
    case 3://cardinal of block rendering
      left = static_cast<double>(blockTested->getNbRendering());
      break;
    default://nothing
      break;
  }
    switch (category_right)
    {
    case 1://Output of block
    {
      ParamValue* tmp = opt_value_right.get<ParamValue*>(false);
      if (tmp != NULL)
        right = *tmp;
      break;
    }
    case 2://Constante value
      right = opt_value_right;
      break;
    case 3://Is empty
      if (boolean_operator == 0)
        return left.isDefaultValue();
      else
        return !left.isDefaultValue();
    default://nothing
      break;
    }

    switch (boolean_operator)
    {
    case 0://==
      return left == right;
    case 1://!=
      return left != right;
    case 2://<
      return left < right;
    case 3://>
      return left > right;
    case 4://<=
      return left <= right;
    case 5://>=
      return left >= right;
    default://nothing
      return true;
    }

    return true;
  }

  void ConditionOfRendering::setValue(bool left, ParamValue* param2)
  {
    if (left)
    {
      switch (category_left)
      {
      case 1://Output of block
      {
        opt_value_left = param2;
        break;
      }
      default://nothing
        opt_value_left = *param2;
      }
    }
    else
    {
      switch (category_right)
      {
      case 1://Output of block
      {
        opt_value_right = param2;
        break;
      }
      default://nothing
        opt_value_right = *param2;
      }
    }
  }

  Block::~Block()
  {
    _processes->extractProcess(this);
    wakeUp();
    wakeUpOutputListeners();
  }

  Block::Block(std::string name, BlockType typeExec){
    _exec_type = typeExec;
    _name = name;
    _timestamp = 0;
    nbRendering = 0;
    _executeOnlyOnce = false;
  };

  void Block::operator()()
  {
    boost::unique_lock<boost::mutex> lock(_mtx);
    nbRendering = 0;
    try
    {
      while (true)//this will stop when user stop the process...
      {
        _renderingSkiped = false;
        while (GraphOfProcess::pauseProcess)
          _cond_pause.wait(lock);//wait for play

        _processes->shouldWaitChild(this);//ask to scheduler if we have to wait...
        bool shouldRun = true;
        for (ConditionOfRendering& condition : _conditions)
        {
          if (!condition.canRender(this))
            shouldRun= false;
        }
        //now we can run the process:
        if (shouldRun && !_renderingSkiped)
        {
          run();
          _processes->blockProduced(this);//tell to scheduler we produced some datas...
        }
        else
          skipRendering();

        nbRendering++;

        _cond_sync.notify_all();//wake up waiting thread (if needed)
        boost::this_thread::sleep(boost::posix_time::milliseconds(10.));
        if (_executeOnlyOnce)
          break;
      }
    }
    catch (boost::thread_interrupted const&)
    {
      //end of thread (requested by interrupt())!
    }
  }

  void Block::newProducedData()
  {
    _processes->blockProduced(this, false);//tell to scheduler we produced some datas...
    if (_exec_type == asynchrone)return;//no need to wait
    //we have to wait entire chain of rendering to process our data:
    _processes->shouldWaitConsumers(this);//ask to scheduler if we have to wait...
  }

  bool Block::isAncestor(Block* other)
  {
    //add empty parameters:
    auto it = _myInputs.begin();
    while (it != _myInputs.end())
    {
      if (it->second.isLinked())
      {
        ParamValue* otherParam = it->second.get<ParamValue*>(false);
        if (otherParam->getBlock() == other)
          return true;
        if (otherParam->getBlock()->isAncestor(other))
          return true;
      }
      it++;
    }
    return false;
  }

  std::vector<cv::String> Block::getSubParams(std::string paramName)
  {
    vector<cv::String> out;
    const std::vector<ParamDefinition>& subParams = _PROCESS_MANAGER->getAlgo_SubParams(_name);
    //test if param exist
    for (auto val : subParams)
    {
      auto pos = val._name.find(paramName);
      if (pos != string::npos && paramName.length() > pos + 1)
        out.push_back(val._helper);
    }
    return out;
  }

  bool Block::isStartingBlock()
  {
    if (ProcessManager::getInstance()->getAlgoType(_name) != input)
      return false;
    auto it = _myInputs.begin();
    while (it != _myInputs.end())
    {
      if (it->second.isLinked())
        return false;
      it++;
    }
    return true;
  }

  void Block::wakeUpOutputListeners()
  {
    //for each output, test if a new value was set
    for (auto it = _myOutputs.begin(); it != _myOutputs.end(); it++)
    {
      //wake up the threads, if any!
      std::set<ParamValue*>& listeners = it->second.getListeners();
      for (auto listener : listeners)
      {
        if (listener->isLinked())
          listener->getBlock()->wakeUp();
      }
    }
  }
  void Block::wakeUp()
  {
    _cond_sync.notify_all();//wake up waiting thread (if needed)
    _param_sync.notify_all();
    _cond_pause.notify_all();
  }

  void Block::skipRendering()
  {
    {
      boost::unique_lock<boost::mutex> guard(_mtx_timestamp_inc);
      _timestamp = _processes->_current_timestamp;
      _renderingSkiped = true;
    }
    //go through outputs and skip it:
    auto it = _myOutputs.begin();
    while (it != _myOutputs.end())
    {
      std::set<ParamValue*>& listeners = it->second.getListeners();
      for (auto listener : listeners)
      {
        if (listener != NULL)
          listener->getBlock()->skipRendering();
      }
      it++;
    }
    _cond_sync.notify_all();//wake up waiting thread (if needed)
  }

  bool Block::isReadyToRun(bool realCheck)
  {
    _error_msg = "";
    for (auto it = _myInputs.begin(); it != _myInputs.end(); it++)
    {
      try
      {
        if (it->second.isLinked())
        {
          ParamValue* other = it->second.get<ParamValue*>(false);
          if (realCheck)
          {
            it->second.validate(*other);
          }
          else
          {
            if (!other->getBlock()->isReadyToRun())
              throw ErrorValidator(other->getBlock()->_error_msg);
          }
        }
        else
          it->second.validate(it->second);
      }
      catch (ErrorValidator& e)
      {
        _error_msg += e.errorMsg + "<br/>";
      }
    }
    return _error_msg.empty();
  }

  bool Block::hasNewParameters()
  {
    for (auto it = _myInputs.begin(); it != _myInputs.end(); it++)
    {
      if (it->second.isNew())
        return true;
    }
    return false;
  }

  void Block::waitUpdateTimestamp(boost::unique_lock<boost::mutex>& lock)
  {
    _param_sync.wait(lock);
  }

  bool Block::validateParams(std::string param, const ParamValue val){
    try
    {
      _myInputs[param].validate(val);
    }
    catch (ErrorValidator& e)
    {
      _error_msg += e.errorMsg + "<br/>";
      return false;
    }
    return true;
  }

  std::string Block::getErrorMsg() {
    std::string tmp = _error_msg;
    _error_msg = "";
    return tmp;
  }

  void Block::initParameters(const std::vector<ParamDefinition>& inParam,
    const std::vector<ParamDefinition>& outParam)
  {
    //add empty parameters:
    auto it = inParam.begin();
    while (it != inParam.end())
    {
      ParamValue& t = _myInputs[it->_name] = ParamValue(this, it->_name, false);
      t.isNeeded(it->_show);
      t = it->_initVal;
      it++;
    }
    it = outParam.begin();
    while (it != outParam.end())
    {
      ParamValue& t = _myOutputs[it->_name] = ParamValue(this, it->_name, true);
      t.isNeeded(it->_show);
      t = it->_initVal;
      it++;
    }
    const std::vector<ParamDefinition>& subParams = _PROCESS_MANAGER->getAlgo_SubParams(_name);
    //test if param is an algo:
    for (auto val : subParams)
    {
      ParamValue& t = _mySubParams[val._name] = ParamValue(this, val._helper, true);
      t = val._initVal;
      t.isNeeded(false);
    }
  }

  void Block::setParamValue(std::string nameParam_, ParamValue* value){
    if (_myInputs.find(nameParam_) != _myInputs.end())
      _myInputs[nameParam_] = value;
  };
  ParamValue* Block::getParam(std::string nameParam_, bool input){
    if (input)
    {
      if (_myInputs.find(nameParam_) != _myInputs.end())
        return &_myInputs[nameParam_];
      //maybe a subparam?
      if (_mySubParams.find(nameParam_) != _mySubParams.end())
        return &_mySubParams[nameParam_];
    }
    else
      if (!input && _myOutputs.find(nameParam_) != _myOutputs.end())
        return &_myOutputs[nameParam_];

    return NULL;
  };

  std::vector<BlockLink> Block::getInEdges()
  {
    vector<BlockLink> out;
    auto it = _myInputs.begin();
    while (it != _myInputs.end())
    {
      if (it->second.isLinked())
        out.push_back(BlockLink(it->second.toBlockLink()));
      it++;
    }
    return out;
  }

  void Block::initFromXML(boost::property_tree::ptree* block,
    std::vector < std::pair<ParamValue*, unsigned int> >& toUpdate,
    std::map<unsigned int, ParamValue*>& addressesMap,
    std::vector<ConditionOfRendering*>& condToUpdate)
  {
    string pos = block->get("position", "[0,0]");
    int posSepare = pos.find_first_of(',') + 1;
    string xPos = pos.substr(1, posSepare - 2);
    string yPos = pos.substr(posSepare + 1, pos.size() - posSepare - 2);

    setPosition(lexical_cast<float>(xPos), lexical_cast<float>(yPos));
    for (ptree::iterator it1 = block->begin(); it1 != block->end(); it1++)
    {
      if (it1->first.compare("Input") == 0)
      {
        string nameIn = it1->second.get("Name", "Error");
        bool link = it1->second.get("Link", false);
        string val = it1->second.get("Value", "Not initialized...");
        ParamValue* tmpVal = getParam(nameIn, true);
        string valID = it1->second.get("ID", "0");
        addressesMap[lexical_cast<unsigned int>(valID)] = tmpVal;

        if (!link)
        {
          try
          {
            if (tmpVal != NULL)
              tmpVal->valid_and_set(tmpVal->fromString(tmpVal->getType(), val));
          }
          catch (...)
          {
          }
        }
        else
          toUpdate.push_back(std::pair<ParamValue*, unsigned int>(tmpVal, lexical_cast<unsigned int>(val)));
      }
      if (it1->first.compare("Output") == 0)
      {
        string nameOut = it1->second.get("Name", "Error");
        string val = it1->second.get("ID", "0");
        ParamValue* tmpVal = getParam(nameOut, false);
        addressesMap[lexical_cast<unsigned int>(val)] = tmpVal;
      }
      if (it1->first.compare("Condition") == 0)
      {
        int cLeft = it1->second.get("category_left", 0);
        int cRight = it1->second.get("category_right", 0);
        int cOperator = it1->second.get("boolean_operator", 0);

        double valLeft = it1->second.get("Value_left", 0.);
        double valRight = it1->second.get("Value_right", 0.);
        addCondition(ConditionOfRendering(cLeft, valLeft, cRight, valRight, cOperator,
          this));
        if (cLeft == 1 || cRight == 1)//output of block...
          condToUpdate.push_back(&_conditions.back());
      }
      if (it1->first.compare("SubParam") == 0)
      {
        string nameIn = it1->second.get("Name", "Error");
        bool link = it1->second.get("Link", false);
        string val = it1->second.get("Value", "Not initialized...");
        ParamValue* tmpVal = getParam(nameIn, true);
        string valID = it1->second.get("ID", "0");
        addressesMap[lexical_cast<unsigned int>(valID)] = tmpVal;

        if (!link)
        {
          try
          {
            if (tmpVal != NULL)
              tmpVal->valid_and_set(tmpVal->fromString(tmpVal->getType(), val));
          }
          catch (...)
          {
          }
        }
        else
          toUpdate.push_back(std::pair<ParamValue*, unsigned int>(tmpVal, lexical_cast<unsigned int>(val)));
      }
    }
  }

  boost::property_tree::ptree Block::getXML()
  {
    ptree tree;
    tree.put("name", _name);
    tree.put("position", _position);
    vector<string> inputWithSubparams;

    for (auto it = _myInputs.begin();
      it != _myInputs.end(); it++)
    {
      ptree paramTree;
      paramTree.put("Name", it->first);
      paramTree.put("ID", (unsigned int)&it->second);
      paramTree.put("Link", it->second.isLinked());
      if (!it->second.isLinked())
        paramTree.put("Value", it->second.toString());
      else
        paramTree.put("Value", (unsigned int)it->second.get<ParamValue*>(false));

      tree.add_child("Input", paramTree);

      if (it->second.getType()==ListBox)
      {
        inputWithSubparams.push_back(it->second.getName() + "." + it->second.getValFromList());
      }
    }

    for (auto it = _myOutputs.begin();
      it != _myOutputs.end(); it++)
    {
      ptree paramTree;
      paramTree.put("Name", it->first);
      paramTree.put("ID", (unsigned int)&it->second);

      tree.add_child("Output", paramTree);
    }

    for (auto it = _mySubParams.begin();
      it != _mySubParams.end(); it++)
    {
      bool shouldAdd = false;
      string paramName = it->first;
      for (auto it1 = inputWithSubparams.begin();
        it1 != inputWithSubparams.end() && !shouldAdd; it1++)
      {
        if (paramName.find(*it1) != string::npos)
          shouldAdd = true;
      }
      if (shouldAdd)
      {
        ptree paramTree;
        paramTree.put("Name", it->first);
        paramTree.put("ID", (unsigned int)&it->second);
        paramTree.put("Link", it->second.isLinked());
        if (!it->second.isLinked())
          paramTree.put("Value", it->second.toString());
        else
          paramTree.put("Value", (unsigned int)it->second.get<ParamValue*>(false));

        tree.add_child("SubParam", paramTree);
      }
    }

    for (auto it = _conditions.begin();
      it != _conditions.end(); it++)
      tree.add_child("Condition", it->getXML());

    return tree;
  };

  void Block::linkParam(std::string paramName, Block* dest, std::string paramNameDest)
  {
    ParamValue& valIn = ParamValue();
    if (dest->_myInputs.find(paramNameDest) == dest->_myInputs.end())
      valIn = dest->_mySubParams[paramNameDest];
    else
      valIn = dest->_myInputs[paramNameDest];

    ParamValue& valOut = _myOutputs[paramName];
    //first test type of input:
    if (valIn.getType() != valOut.getType())
    {
      throw (ErrorValidator((my_format(_STR("ERROR_TYPE")) %
        _STR(getName()) % _STR(valOut.getName()) % typeName(valOut.getType()) %
        _STR(dest->getName()) % _STR(valIn.getName()) % typeName(valIn.getType())).str()));
    }
    dest->setParamValue(paramNameDest, &_myOutputs[paramName]);
  }

  void Block::setPosition(int x, int y)
  {
    _position.x = x;
    _position.y = y;
  }

  void Block::removeCondition()
  {
    _conditions.pop_back();
  }

  vector<ParamDefinition> Block::getInParams() const
  {
    return ProcessManager::getInstance()->getAlgo_InParams(_name);
  };
  vector<ParamDefinition> Block::getOutParams() const
  {
    return ProcessManager::getInstance()->getAlgo_OutParams(_name);
  };
}