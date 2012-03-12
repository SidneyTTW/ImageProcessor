#include "drawlineprocessor.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QSpinBox>
#include "imageviewwidget.h"

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

MyImage *DrawLineProcessor::processImage(const MyImage& image) const
{
  QImage *resultImage = new QImage(image.getImage());
  if (valid)
  {
    QPainter *painter = new QPainter(resultImage);
    QPen pen(getCurrentColor());
    pen.setWidth(_width);
    painter->setPen(pen);
    painter->drawLine(startPosition, endPosition);
    painter->end();
    delete painter;
  }
  MyImage *result = new MyImage(*resultImage, MyImage::Colored);
  delete resultImage;
  return result;
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
    QPen pen(getCurrentColor());
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

bool DrawLineProcessor::eventFilter(QObject *object, QEvent *event)
{
  ImageViewWidget *imageViewer = (ImageViewWidget *) object;
  switch (event->type())
  {
  case QEvent::MouseButtonPress:
    startPosition = imageViewer->toImagePosition(((QMouseEvent *) event)->pos());
    endPosition = startPosition;
    valid = true;
    break;
  case QEvent::MouseMove:
    endPosition = imageViewer->toImagePosition(((QMouseEvent *) event)->pos());
    emit optionChanged(this);
    break;
  case QEvent::MouseButtonRelease:
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
