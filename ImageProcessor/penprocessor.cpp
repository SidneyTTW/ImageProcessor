#include "penprocessor.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QSpinBox>
#include "imageviewwidget.h"

PenProcessor::PenProcessor() :
    _width(1),
    valid(false),
    _optionWidget(NULL)
{
}

MyImage::ImageTypeFlag PenProcessor::resultType() const
{
  return (MyImage::ImageTypeFlag)MyImage::Colored;
}

QImage *PenProcessor::processImage(const QImage& image) const
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
    painter->drawPoints(positions);
    painter->end();
    delete painter;
  }
  return result;
}

void PenProcessor::processImage(QImage *image) const
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
    painter->drawPoints(positions);
    painter->end();
    delete painter;
  }
}

QWidget *PenProcessor::optionWidget()
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

MyImage PenProcessor::preProcessImage(const MyImage& image) const
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
    painter->drawPoints(positions);
    painter->end();
    delete painter;
  }
  MyImage result(*resultImage, MyImage::Colored);
  delete resultImage;
  return result;
}

bool PenProcessor::cancelWhenNewOneIsCreated() const
{
  return false;
}

void PenProcessor::interrupt()
{
  valid = false;
}

QString PenProcessor::name() const
{
  return "Pen";
}

QString PenProcessor::toString() const
{
  QColor color = _color;
  if (getCurrentColor().isValid())
    color = getCurrentColor();
  QString result = tr("%1 %2").arg(color.rgba()).arg(_width);
  for (int i = 0;i < positions.size();++i)
    result += tr(" %1 %2").arg(positions[i].x()).arg(positions[i].y());
  return result;
}

AbstractImageProcessor *PenProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() <= 2 || list.size() % 2 == 1)
    return NULL;
  PenProcessor *result = new PenProcessor();
  QRgb rgb = list.takeFirst().toUInt();
  result->_color = QColor(rgb);
  result->_width = list.takeFirst().toInt();
  result->valid = true;
  while (!list.isEmpty())
    result->positions.push_back(QPoint(list.takeFirst().toInt(),
                                       list.takeFirst().toInt()));
  return result;
}

bool PenProcessor::eventFilter(QObject *object, QEvent *event)
{
  ImageViewWidget *imageViewer = (ImageViewWidget *) object;
  switch (event->type())
  {
  case QEvent::MouseButtonPress:
    positions.clear();
    positions.push_back(
        imageViewer->toImagePosition(((QMouseEvent *) event)->pos()));
    valid = true;
    break;
  case QEvent::MouseMove:
    positions.push_back(
        imageViewer->toImagePosition(((QMouseEvent *) event)->pos()));
    emit optionChanged(this);
    break;
  case QEvent::MouseButtonRelease:
    {
      PenProcessor *newProcessor = new PenProcessor();
      newProcessor->_width = _width;
      newProcessor->valid = valid;
      newProcessor->_colorChooser = _colorChooser;
      newProcessor->positions = positions;
      emit processorCreated(newProcessor);
      valid = false;
    }
    break;
  default:
    break;
  }
  return false;
}

void PenProcessor::changeToWidth(int width)
{
  _width = width;
}
