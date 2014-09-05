
#include <vector>
#include <sstream>

#include "Block.h"
#include "window_QT.h"
#include "ParamValidator.h"
using namespace lsis_org;
using std::vector;
using std::string;
using cv::Mat;

namespace charliesoft
{
  BLOCK_BEGIN_INSTANTIATION(BlockShow);
  //You can add methods, re implement needed functions...
  BLOCK_END_INSTANTIATION(BlockShow, AlgoType::output, BLOCK__OUTPUT_NAME);

  BEGIN_BLOCK_INPUT_PARAMS(BlockShow);
  //Add parameters, with following parameters:
  //default visibility, type of parameter, name (key of internationalizor), helper...
  ADD_PARAMETER(true, Mat, "BLOCK__OUTPUT_IN_IMAGE", "BLOCK__OUTPUT_IN_IMAGE_HELP");
  ADD_PARAMETER(false, String, "BLOCK__OUTPUT_IN_WIN_NAME", "BLOCK__OUTPUT_IN_WIN_NAME_HELP");
  END_BLOCK_PARAMS();

  BEGIN_BLOCK_OUTPUT_PARAMS(BlockShow);
  END_BLOCK_PARAMS();

  BlockShow::BlockShow() :Block("BLOCK__OUTPUT_NAME"){
    _myInputs["BLOCK__OUTPUT_IN_IMAGE"].addValidator({ new ValNeeded() });
  };
  
  bool BlockShow::run(){
    if (_myInputs["BLOCK__OUTPUT_IN_IMAGE"].isDefaultValue())
      return false;
    cv::Mat mat = _myInputs["BLOCK__OUTPUT_IN_IMAGE"].get<cv::Mat>();
    if (!mat.empty())
      charliesoft::imshow(_myInputs["BLOCK__OUTPUT_IN_WIN_NAME"].get<string>(), mat);
    renderingDone();
    return true;
  };
};