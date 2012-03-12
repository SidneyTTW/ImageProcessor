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

MyImage *PenProcessor::processImage(const MyImage& image) const
{
  QImage *resultImage = new QImage(image.getImage());
  if (valid)
  {
    QPainter *painter = new QPainter(resultImage);
    QPen pen(getCurrentColor());
    pen.setWidth(_width);
    painter->setPen(pen);
    painter->drawPoints(positions);
    painter->end();
    delete painter;
  }
  MyImage *result = new MyImage(*resultImage, MyImage::Colored);
  delete resultImage;
  return result;
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
    QPen pen(getCurrentColor());
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
