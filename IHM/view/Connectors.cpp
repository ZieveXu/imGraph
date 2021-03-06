﻿
#include "Connectors.h"

#include <boost/algorithm/string.hpp>

#include "Window.h"
#include "GraphicView.h"
#include "VertexRepresentation.h"

namespace charliesoft
{
  const qreal Pi = 3.1415926;

  bool lineIntersect(QLineF& line1, double x1, double y1, double x2, double y2)
  {
    QLineF line;
    line.setP1(QPointF(x1, y1));
    line.setP2(QPointF(x2, y2));
    QPointF intersection;
    return line.intersect(line1, &intersection) == QLineF::BoundedIntersection;
  }

  LinkPath::LinkPath(LinkConnexionRepresentation* src,
    LinkConnexionRepresentation* dst,
    QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsLineItem(parent){
    _src = src; _dst = dst; _selected = false;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setLine(QLineF(_dst->getWorldAnchor(), _src->getWorldAnchor()));
  };

  QPainterPath LinkPath::shape() const
  {
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(_arrowHead);
    return path;
  }

  bool LinkPath::intersect(const QRect& rect) const
  {
    QLineF line;
    QPoint p1 = _src->getWorldAnchor();
    QPoint p2 = _dst->getWorldAnchor();
    if (rect.contains(p1.x(), p1.y()) || rect.contains(p2.x(), p2.y()))
      return true;
    line.setP1(QPointF(p1.x(), p1.y()));
    line.setP2(QPointF(p2.x(), p2.y()));
    return lineIntersect(line, rect.x(), rect.y(), rect.x() + rect.width(), rect.y()) ||
      lineIntersect(line, rect.x(), rect.y(), rect.x(), rect.y() + rect.height()) ||
      lineIntersect(line, rect.x(), rect.y() + rect.height(), rect.x() + rect.width(), rect.y() + rect.height());
  }

  void LinkPath::draw(QPainter *painter, const QStyleOptionGraphicsItem *,
    QWidget *)
  {
    QPen myPen = pen();
    QColor myColor;
    if (_selected)
      myColor = QColor(255, 0, 0);
    else
      myColor = Qt::black;
    myPen.setColor(myColor);
    qreal arrowSize = 10;
    painter->setPen(myPen);
    painter->setBrush(myColor);

    QLineF centerLine(_src->pos(), _dst->pos());

    setLine(QLineF(_dst->getWorldAnchor(), _src->getWorldAnchor()));

    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
      angle = (Pi * 2) - angle;

    QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 2.5) * arrowSize,
      cos(angle + Pi / 2.5) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 2.5) * arrowSize,
      cos(angle + Pi - Pi / 2.5) * arrowSize);

    _arrowHead.clear();
    _arrowHead << line().p1() << arrowP1 << arrowP2;

    painter->drawLine(line());
    painter->drawPolygon(_arrowHead);
    
    if (_selected) {
      painter->setPen(QPen(myColor, 1, Qt::DashLine));
      QLineF myLine = line();
      myLine.translate(0, 4.0);
      painter->drawLine(myLine);
      myLine.translate(0, -8.0);
      painter->drawLine(myLine);
    }
    
  }

  ConditionLinkRepresentation::ConditionLinkRepresentation(ConditionOfRendering* model, bool isLeftCond, QWidget *father) :
    LinkConnexionRepresentation(_STR(isLeftCond ? "CONDITION_BLOCK_LEFT" : "CONDITION_BLOCK_RIGHT"), isLeftCond, father), _model(model){
    setObjectName("ParamRepresentation");
    setToolTip("<FONT>" + _QT("CONDITION_BLOCK_HELP") + "</FONT>");

    connect(this, SIGNAL(askSynchro()), (VertexRepresentation*)father->parentWidget(), SLOT(reshape()));
  }


  ParamRepresentation::ParamRepresentation(ParamRepresentation* other) :
    ParamRepresentation(other->_model, other->_param, other->_isInput, other->_father)
  {
    _defaultValue = other->_defaultValue;
    _isSubParam = other->_isSubParam;
  }

  ParamRepresentation::ParamRepresentation(Block* model, ParamDefinition* param, bool isInput, QWidget *father) :
    LinkConnexionRepresentation(param->_name, isInput, father), _model(model), _param(param), _paramType(_param->_type){
    _defaultValue = _isSubParam = false;
    setObjectName("ParamRepresentation");
    if (!param->_show) this->hide();
    setToolTip("<FONT>" + _QT(param->_helper) + "</FONT>");
    if (_vertex != NULL)
      connect(this, SIGNAL(askSynchro()), _vertex, SLOT(reshape()));
  };

  SubGraphParamRepresentation::SubGraphParamRepresentation(SubBlock* model, const ParamDefinition* def, bool isInput, QWidget *father) :
    LinkConnexionRepresentation(def->_name, isInput, father), _model(model), _param(*def){
    setObjectName("SubGraphParamRepresentation");
    setToolTip("<FONT>" + _QT(def->_helper) + "</FONT>");
    setAlignment(Qt::AlignCenter);
  }

  std::string ParamRepresentation::getParamHelper() const {
    size_t pos = _STR(_param->_helper).find_first_of('|');
    if (pos == std::string::npos)
      return _param->_helper;
    else
      return _STR(_param->_helper).substr(0, pos);
  }

  std::vector<std::string> ParamRepresentation::getParamListChoice() const
  {
    std::vector<std::string> out;
    size_t pos = _STR(_param->_helper).find_first_of('|');
    if (pos == std::string::npos)
      return out;
    std::string params = _STR(_param->_helper).substr(pos+1);
    boost::split(out, params, boost::is_any_of("^"));
    return out;
  }

  void ParamRepresentation::setVisibility(ParamVisibility visible)
  {
    if (_param->_show == visible)
      return;//Nothing to do...

    getParamValue()->isNeeded(visible != notUsed);
    _param->_show = visible;
    emit askSynchro();
  }

  void ParamRepresentation::setSubParam(bool param1)
  {
    _isSubParam = param1;///\todo something is wrong here...
    if (_isSubParam)
      setText(_param->_helper.c_str());
    else
      setText(_param->_helper.c_str());
  }

  void ParamRepresentation::redefineParam(std::string fullSubName, ParamType newType)
  {
    _isSubParam = true;
    _subName = fullSubName;
    _paramType = newType;
  }

  LinkConnexionRepresentation::LinkConnexionRepresentation(std::string text, bool isInput, QWidget *father) :
    QLabel(_QT(text), father), _isInput(isInput)
  {
    _father = father;
    QWidget* tmp = father->parentWidget();
    if (father!=NULL)
      _vertex = dynamic_cast<VertexRepresentation*>(father->parentWidget());
    else
      _vertex = NULL;
    _activeVertex = NULL;
    _name = text;
  };
  QPoint LinkConnexionRepresentation::getWorldAnchor()
  {
    QPoint p = parentWidget()->mapTo(Window::getInstance()->getMainWidget(), mapTo(parentWidget(), pos()) / 2);
    if (_isInput)
      return QPoint(p.x(), (int)(p.y() + height() / 2.));
    else
      return QPoint(p.x() + width(), (int)(p.y() + height() / 2.));
  }

  void LinkConnexionRepresentation::setVisibility(bool visible)
  {
    if (_shouldShow == visible)
      return;//Nothing to do...

    _shouldShow = visible;
    emit askSynchro();
  }

  void LinkConnexionRepresentation::changeStyleProperty(const char* propertyName, QVariant val)
  {
    setProperty(propertyName, val);
    style()->unpolish(this);
    style()->polish(this);
    update();
  }

  void LinkConnexionRepresentation::mousePressEvent(QMouseEvent *e)
  {
    if (_vertex == NULL) return;//Nothing to do...

    _activeVertex = _vertex;
    _activeVertex->setParamActiv(this);

    setCursor(Qt::CrossCursor);
    //get the position of widget inside main widget and send signal of a new link creation:
    emit creationLink(getWorldAnchor());
    e->ignore();
  }

  void LinkConnexionRepresentation::mouseReleaseEvent(QMouseEvent *e)
  {
    if (_activeVertex != NULL)
      _activeVertex->setParamActiv(NULL);

    _activeVertex = NULL;
    setCursor(Qt::ArrowCursor);

    emit releaseLink(Window::getInstance()->getMainWidget()->mapFromGlobal(e->globalPos()));
  };
  void LinkConnexionRepresentation::mouseMoveEvent(QMouseEvent *me)
  {
    me->ignore();
  };

  void LinkConnexionRepresentation::enterEvent(QEvent *)
  {
    setCursor(Qt::CrossCursor);
  }

}