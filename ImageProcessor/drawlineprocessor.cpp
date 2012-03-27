#include "drawlineprocessor.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QSpinBox>

DrawLineProcessor::DrawLineProcessor() :
    _width(1),
    valid(false),
    _optionWidget(NULL)
{
}

MyImage::ImageTypeFlag DrawLineProcessor::resultType() const
{
  return (MyImage::ImageTypeFlag)MyImage::Colored;
}

QImage *DrawLineProcessor::processImage(const QImage& image) const
{
  QImage *result = new QImage(image);
  if (valid)
  {
    QPainter *painter = new QPainter(result);
    QPen pen;
    if (getCurrentColor().isValid())
      pen = QPen(getCurrentColor());
    else
      pen = QPen(_color);
    pen.setWidth(_width);
    painter->setPen(pen);
    painter->drawLine(startPosition, endPosition);
    painter->end();
    delete painter;
  }
  return result;
}

void DrawLineProcessor::processImage(QImage *image) const
{
  if (valid)
  {
    QPainter *painter = new QPainter(image);
    QPen pen;
    if (getCurrentColor().isValid())
      pen = QPen(getCurrentColor());
    else
      pen = QPen(_color);
    pen.setWidth(_width);
    painter->setPen(pen);
    painter->drawLine(startPosition, endPosition);
    painter->end();
    delete painter;
  }
}

QWidget *DrawLineProcessor::optionWidget()
{
  if (_optionWidget == NULL)
  {
    _optionWidget = new QSpinBox();
    _optionWidget->setMinimum(1);
    _optionWidget->setMaximum(100);
    connect(_optionWidget,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(changeToWidth(int)));
  }
  return _optionWidget;
}

MyImage DrawLineProcessor::preProcessImage(const MyImage& image) const
{
  QImage *resultImage = new QImage(image.getImage());
  if (valid)
  {
    QPainter *painter = new QPainter(resultImage);
    QPen pen;
    if (getCurrentColor().isValid())
      pen = QPen(getCurrentColor());
    else
      pen = QPen(_color);
    pen.setWidth(_width);
    painter->setPen(pen);
    painter->drawLine(startPosition, endPosition);
    painter->end();
    delete painter;
  }
  MyImage result(*resultImage, MyImage::Colored);
  delete resultImage;
  return result;
}

bool DrawLineProcessor::cancelWhenNewOneIsCreated() const
{
  return false;
}

void DrawLineProcessor::interrupt()
{
  valid = false;
}

QString DrawLineProcessor::name() const
{
  return "Line";
}

QString DrawLineProcessor::toString() const
{
  QColor color = _color;
  if (getCurrentColor().isValid())
    color = getCurrentColor();
  return tr("%1 %2 %3 %4 %5 %6").arg(color.rgba()).
                                 arg(_width).
                                 arg((startPosition.x())).
                                 arg((startPosition.y())).
                                 arg((endPosition.x())).
                                 arg((endPosition.y()));
}

AbstractImageProcessor *DrawLineProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() != 6)
    return NULL;
  DrawLineProcessor *result = new DrawLineProcessor();
  QRgb rgb = list.takeFirst().toUInt();
  result->_color = QColor(rgb);
  result->_width = list.takeFirst().toInt();
  result->startPosition.setX(list.takeFirst().toInt());
  result->startPosition.setY(list.takeFirst().toInt());
  result->endPosition.setX(list.takeFirst().toInt());
  result->endPosition.setY(list.takeFirst().toInt());
  result->valid = true;
  return result;
}

bool DrawLineProcessor::eventFilter(QObject *object, QEvent *event)
{
  switch (event->type())
  {
  case QEvent::GraphicsSceneMousePress:
    {
      QPointF pos = ((QGraphicsSceneMouseEvent *) event)->scenePos();
      startPosition = QPoint(pos.x(), pos.y());
      endPosition = startPosition;
      valid = true;
      break;
    }
  case QEvent::GraphicsSceneMouseMove:
    {
      QPointF pos = ((QGraphicsSceneMouseEvent *) event)->scenePos();
      endPosition = QPoint(pos.x(), pos.y());
      emit optionChanged(this);
      break;
    }
  case QEvent::GraphicsSceneMouseRelease:
    {
      DrawLineProcessor *newProcessor = new DrawLineProcessor();
      newProcessor->_width = _width;
      newProcessor->valid = valid;
      newProcessor->_colorChooser = _colorChooser;
      newProcessor->startPosition = startPosition;
      newProcessor->endPosition = endPosition;
      emit processorCreated(newProcessor);
      valid = false;
    }
    break;
  default:
    break;
  }
  return false;
}

void DrawLineProcessor::changeToWidth(int width)
{
  _width = width;
}
