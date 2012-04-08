#include "tograyprocessor.h"

#include "tograyoptionwidget.h"

ToGrayProcessor::ToGrayProcessor() :
    _type(ImageAlgorithm::Float),
    _optionWidget(NULL)
{
}

MyImage::ImageTypeFlag ToGrayProcessor::resultType() const
{
  return (MyImage::ImageTypeFlag)MyImage::Gray;
}

QImage *ToGrayProcessor::processImage(const QImage& image) const
{
  QImage *result = ImageAlgorithm::convertToGrayScale(image, _type);
  return result;
}

void ToGrayProcessor::processImage(QImage *image) const
{
  ImageAlgorithm::convertToGrayScale(image, _type);
}

QWidget *ToGrayProcessor::optionWidget()
{
  if (_optionWidget == NULL)
  {
    _optionWidget = new ToGrayOptionWidget(_type);
    connect(_optionWidget,
            SIGNAL(optionChanged(ImageAlgorithm::ImageToGrayAlgorithmType)),
            this,
            SLOT(changeToType(ImageAlgorithm::ImageToGrayAlgorithmType)));
    connect(_optionWidget, SIGNAL(confirm()), this, SLOT(confirm()));
  }
  return _optionWidget;
}

MyImage ToGrayProcessor::preProcessImage(const MyImage& image) const
{
  QImage *resultImage = ImageAlgorithm::convertToGrayScale(image.getImage(),
                                                           _type);
  MyImage result(*resultImage, MyImage::Gray);
  delete resultImage;
  return result;
}

bool ToGrayProcessor::cancelWhenNewOneIsCreated() const
{
  return true;
}

void ToGrayProcessor::interrupt()
{
}

QString ToGrayProcessor::name() const
{
  return "Gray";
}

QString ToGrayProcessor::toString() const
{
  return tr("%1").arg((int)_type);
}

AbstractImageProcessor *ToGrayProcessor::fromString(const QString& str) const
{
  ToGrayProcessor *result = new ToGrayProcessor();
  result->_type = (ImageAlgorithm::ImageToGrayAlgorithmType) str.toInt();
  return result;
}

QString ToGrayProcessor::description() const
{
  return "Turn the image into gray";
}

QString ToGrayProcessor::iconPath() const
{
  return Resource::iconGray;
}

void ToGrayProcessor::confirm()
{
  ToGrayProcessor *newProcessor = new ToGrayProcessor();
  newProcessor->_type = _type;
  emit processorCreated(newProcessor);
}

void ToGrayProcessor::changeToType(ImageAlgorithm::ImageToGrayAlgorithmType
                                   type)
{
  _type = type;
  emit optionChanged(this);
}
