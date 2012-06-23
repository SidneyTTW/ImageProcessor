#include "skeletonprocessor.h"

#include "imagealgorithm.h"

Area::AreaType SkeletonProcessor::acceptableAreaType() const
{
  return Area::TypeEmpty;
}

Area::AreaTypeFlag SkeletonProcessor::resultAreaType() const
{
  return Area::TypeEmpty;
}

Area SkeletonProcessor::resultArea() const
{
  return Area();
}

MyImage::ImageType SkeletonProcessor::acceptableType() const
{
  return MyImage::BlackAndWhite;
}

MyImage::ImageTypeFlag SkeletonProcessor::resultType() const
{
  return MyImage::Gray;
}

QImage *SkeletonProcessor::processImage(const QImage& image) const
{
  QImage *result = ImageAlgorithm::skeleton(image);
  return result;
}

void SkeletonProcessor::processImage(QImage *image) const
{
  ImageAlgorithm::skeleton(image);
}

QDialog *SkeletonProcessor::getOptionDialog(Area area, const MyImage& image)
{
  emit processorCreated(new SkeletonProcessor());
  return NULL;
}

QString SkeletonProcessor::name() const
{
  return "Skeleton";
}

QString SkeletonProcessor::toString() const
{
  return " ";
}

AbstractImageProcessor *SkeletonProcessor::fromString(const QString& str) const
{
  return new SkeletonProcessor();
}

QString SkeletonProcessor::description() const
{
  return "Skeleton";
}

QString SkeletonProcessor::iconPath() const
{
  return "";
}
