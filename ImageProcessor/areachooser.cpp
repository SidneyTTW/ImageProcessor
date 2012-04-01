#include "areachooser.h"

#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include "imageviewwidget.h"

AreaChooser::AreaChooser(QObject *parent) :
    QObject(parent),
    _widget(NULL),
    _type(TypePolygon)
{
}

void AreaChooser::attach(ImageViewWidget *widget)
{
  if (_widget != NULL)
  {
    _widget->setArea(Area());
    _widget->removeEventFilter(this);
  }
  _widget = widget;
  _widget->installEventFilter(this);
  keyPoints.clear();
}

void AreaChooser::detach()
{
  if (_widget != NULL)
  {
    _widget->setArea(Area());
    _widget->removeEventFilter(this);
  }
  _widget = NULL;
  keyPoints.clear();
}

void AreaChooser::setType(AreaChooseType type)
{
  _type = type;
  if (_widget != NULL)
    _widget->setArea(Area());
  keyPoints.clear();
}

bool AreaChooser::eventFilter(QObject *object, QEvent *event)
{
  if (object != _widget->getScene())
    return false;
  switch (event->type())
  {
  case QEvent::GraphicsSceneMousePress:
    {
      QPointF pos = ((QGraphicsSceneMouseEvent *) event)->scenePos();
      if (_type == TypeRectangle)
      {
        keyPoints.clear();
        keyPoints.push_back(QPoint(pos.x(), pos.y()));
        keyPoints.push_back(QPoint(pos.x(), pos.y()));
        _widget->setArea(Area(QRect(keyPoints[0], keyPoints[1])));
      }
      else if (_type == TypePolygon)
      {
        keyPoints.push_back(QPoint(pos.x(), pos.y()));
        _widget->setArea(Area(QPolygon(keyPoints)));
      }
      else if (_type == TypeEllipse)
      {
        keyPoints.clear();
        keyPoints.push_back(QPoint(pos.x(), pos.y()));
        keyPoints.push_back(QPoint(pos.x(), pos.y()));
        _widget->setArea(Ellipse(QPoint((keyPoints[0].x() + keyPoints[1].x()) / 2,
                                        (keyPoints[0].y() + keyPoints[1].y()) / 2),
                                 (keyPoints[1].x() - keyPoints[0].x()) / 2,
                                 (keyPoints[1].y() - keyPoints[0].y()) / 2));
      }
    }
    break;
  case QEvent::GraphicsSceneMouseMove:
    if (((QGraphicsSceneMouseEvent *) event)->buttons() == Qt::NoButton)
      break;
    {
      QPointF pos = ((QGraphicsSceneMouseEvent *) event)->scenePos();
      if (_type == TypeRectangle)
      {
        if (((QGraphicsSceneMouseEvent *) event)->modifiers().
            testFlag(Qt::ShiftModifier))
        {
          int a = pos.x() - keyPoints[0].x();
          keyPoints[1] = QPoint(keyPoints[0].x() + a, keyPoints[0].y() + a);
        }
        else
          keyPoints[1] = QPoint(pos.x(), pos.y());
        _widget->setArea(Area(QRect(keyPoints[0], keyPoints[1])));
      }
      else if (_type == TypePolygon)
      {
        keyPoints.last() = QPoint(pos.x(), pos.y());
        _widget->setArea(Area(QPolygon(keyPoints)));
      }
      else if (_type == TypeEllipse)
      {
        if (((QGraphicsSceneMouseEvent *) event)->modifiers().
            testFlag(Qt::ShiftModifier))
        {
          int a = pos.x() - keyPoints[0].x();
          keyPoints[1] = QPoint(keyPoints[0].x() + a, keyPoints[0].y() + a);
        }
        else
          keyPoints[1] = QPoint(pos.x(), pos.y());
        _widget->setArea(Ellipse(QPoint((keyPoints[0].x() + keyPoints[1].x()) / 2,
                                        (keyPoints[0].y() + keyPoints[1].y()) / 2),
                                 (keyPoints[1].x() - keyPoints[0].x()) / 2,
                                 (keyPoints[1].y() - keyPoints[0].y()) / 2));
      }
    }
    break;
  default:
    break;
  }
  return false;
}
