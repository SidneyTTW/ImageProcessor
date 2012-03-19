#include "reverseprocessor.h"

#include "imagealgorithm.h"

ReverseProcessor::ReverseProcessor() :
    _optionWidget(NULL)
{
}

MyImage::ImageTypeFlag ReverseProcessor::resultType() const
{
  return (MyImage::ImageTypeFlag)MyImage::Remain;
}

QImage *ReverseProcessor::processImage(const QImage& image) const
{
  QImage *result = ImageAlgorithm::reverse(image);
  return result;
}

void ReverseProcessor::processImage(QImage *image) const
{
  ImageAlgorithm::reverse(image);
}

QWidget *ReverseProcessor::optionWidget()
{
  if (_optionWidget == NULL)
  {
    _optionWidget = new QPushButton("confirm");
    connect(_optionWidget, SIGNAL(clicked()), this, SLOT(confirm()));
  }
  return _optionWidget;
}

MyImage ReverseProcessor::preProcessImage(const MyImage& image) const
{
  QImage *resultImage = ImageAlgorithm::reverse(image.getImage());
  MyImage result(*resultImage, image.getType());
  delete resultImage;
  return result;
}

bool ReverseProcessor::cancelWhenNewOneIsCreated() const
{
  return true;
}

void ReverseProcessor::interrupt()
{
}

QString ReverseProcessor::name() const
{
  return "Reverse";
}

QString ReverseProcessor::toString() const
{
  return "";
}

AbstractImageProcessor *ReverseProcessor::fromString(const QString& str) const
{
  ReverseProcessor *result = new ReverseProcessor();
  return result;
}

void ReverseProcessor::confirm()
{
  ReverseProcessor *newProcessor = new ReverseProcessor();
  emit processorCreated(newProcessor);
}
