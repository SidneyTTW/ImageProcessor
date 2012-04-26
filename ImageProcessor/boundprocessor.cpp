#include "boundprocessor.h"

#include "imagealgorithm.h"
#include "bounddialog.h"

const int BoundProcessor::UpDownConvolutionCore[9] = {-1, -1, -1,
                                                       0,  0,  0,
                                                       1,  1,  1};

const int BoundProcessor::DownUpConvolutionCore[9] = { 1,  1,  1,
                                                       0,  0,  0,
                                                      -1, -1, -1};

const int BoundProcessor::LeftRightConvolutionCore[9] = {-1,  0,  1,
                                                         -1,  0,  1,
                                                         -1,  0,  1};

const int BoundProcessor::RightLeftConvolutionCore[9] = {1,  0, -1,
                                                         1,  0, -1,
                                                         1,  0, -1};

BoundProcessor::BoundProcessor() {}

Area::AreaType BoundProcessor::acceptableAreaType() const
{
  return Area::TypeEmpty;
}

Area::AreaTypeFlag BoundProcessor::resultAreaType() const
{
  return Area::TypeEmpty;
}

Area BoundProcessor::resultArea() const
{
  return Area();
}

MyImage::ImageType BoundProcessor::acceptableType() const
{
  return MyImage::Gray;
}

MyImage::ImageTypeFlag BoundProcessor::resultType() const
{
  return MyImage::Gray;
}

QImage *BoundProcessor::processImage(const QImage& image) const
{
  QImage *result = new QImage(image.width(), image.height(), QImage::Format_ARGB32);
  result->fill(qRgb(0, 0, 0));
  QImage *boundImage;
  if (type.testFlag(UpDown))
  {
    ImageAlgorithm::ConvolutionFilter *filter =
        new ImageAlgorithm::ConvolutionFilter(UpDownConvolutionCore,
                                              3,
                                              3,
                                              1,
                                              0);
    boundImage = ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(image, Area(), filter);
    delete filter;
    if (boundImage)
    {
      ImageAlgorithm::algebraOperation(result, *boundImage, ImageAlgorithm::Add);
      delete boundImage;
    }
  }
  if (type.testFlag(DownUp))
  {
    ImageAlgorithm::ConvolutionFilter *filter =
        new ImageAlgorithm::ConvolutionFilter(DownUpConvolutionCore,
                                              3,
                                              3,
                                              1,
                                              0);
    boundImage = ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(image, Area(), filter);
    delete filter;
    if (boundImage)
    {
      ImageAlgorithm::algebraOperation(result, *boundImage, ImageAlgorithm::Add);
      delete boundImage;
    }
  }
  if (type.testFlag(LeftRight))
  {
    ImageAlgorithm::ConvolutionFilter *filter =
        new ImageAlgorithm::ConvolutionFilter(LeftRightConvolutionCore,
                                              3,
                                              3,
                                              1,
                                              0);
    boundImage = ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(image, Area(), filter);
    delete filter;
    if (boundImage)
    {
      ImageAlgorithm::algebraOperation(result, *boundImage, ImageAlgorithm::Add);
      delete boundImage;
    }
  }
  if (type.testFlag(RightLeft))
  {
    ImageAlgorithm::ConvolutionFilter *filter =
        new ImageAlgorithm::ConvolutionFilter(RightLeftConvolutionCore,
                                              3,
                                              3,
                                              1,
                                              0);
    boundImage = ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(image, Area(), filter);
    delete filter;
    if (boundImage)
    {
      ImageAlgorithm::algebraOperation(result, *boundImage, ImageAlgorithm::Add);
      delete boundImage;
    }
  }
  return result;
}

void BoundProcessor::processImage(QImage *image) const
{
  QImage *result = processImage(*image);
  *image = *result;
  delete result;
}

QDialog *BoundProcessor::getOptionDialog(Area area,
                                         const MyImage& image)
{
  BoundDialog *result = new BoundDialog(image.getImage());
  connect(result,
          SIGNAL(confrim(BoundProcessor::BoundType)),
          this,
          SLOT(confirm(BoundProcessor::BoundType)));
  return result;
}

QString BoundProcessor::name() const
{
  return "Bound";
}

QString BoundProcessor::toString() const
{
  QString result = tr("%1").arg((int) type);
  return result;
}

AbstractImageProcessor *BoundProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() != 1)
    return NULL;
  BoundProcessor *result = new BoundProcessor();
  result->type = (BoundType) list.takeFirst().toInt();
  return result;
}

QString BoundProcessor::description() const
{
  return "Test all the bounds of the image";
}

QString BoundProcessor::iconPath() const
{
  return Resource::iconBound;
}

void BoundProcessor::confirm(BoundProcessor::BoundType type)
{
  BoundProcessor *processor = new BoundProcessor();
  processor->type = type;
  emit processorCreated(processor);
}
