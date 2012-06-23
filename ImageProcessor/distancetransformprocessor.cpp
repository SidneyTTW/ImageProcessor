#include "distancetransformprocessor.h"

#include "distancetransformdialog.h"

DistanceTransformProcessor::DistanceTransformProcessor()
{
}

Area::AreaType DistanceTransformProcessor::acceptableAreaType() const
{
  return Area::TypeEmpty;
}

Area::AreaTypeFlag DistanceTransformProcessor::resultAreaType() const
{
  return Area::TypeEmpty;
}

Area DistanceTransformProcessor::resultArea() const
{
  return Area();
}

MyImage::ImageType DistanceTransformProcessor::acceptableType() const
{
  return MyImage::BlackAndWhite;
}

MyImage::ImageTypeFlag DistanceTransformProcessor::resultType() const
{
  return MyImage::Gray;
}

QImage *DistanceTransformProcessor::processImage(const QImage& image) const
{
  QImage *result = ImageAlgorithm::distanceTransform(image, type, size);
  return result;
}

void DistanceTransformProcessor::processImage(QImage *image) const
{
  ImageAlgorithm::distanceTransform(image, type, size);
}

QDialog *DistanceTransformProcessor::getOptionDialog(Area area, const MyImage& image)
{
  DistanceTransformDialog *result = new DistanceTransformDialog();
  connect(result,
          SIGNAL(confirm(ImageAlgorithm::DistanceTransformType,int)),
          this,
          SLOT(confirm(ImageAlgorithm::DistanceTransformType,int)));
  return result;
}

QString DistanceTransformProcessor::name() const
{
  return "Distance Transform";
}

QString DistanceTransformProcessor::toString() const
{
  QString result = tr("%1 %2").
                   arg((int) type).
                   arg(size);
  return result;
}

AbstractImageProcessor *DistanceTransformProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() != 2)
    return NULL;
  DistanceTransformProcessor *result = new DistanceTransformProcessor();
  result->type = (ImageAlgorithm::DistanceTransformType) list.takeFirst().toInt();
  result->size = list.takeFirst().toInt();
  return result;
}

QString DistanceTransformProcessor::description() const
{
  return "Distance transform";
}

QString DistanceTransformProcessor::iconPath() const
{
  return "";
}

void DistanceTransformProcessor::confirm(ImageAlgorithm::DistanceTransformType type,
                                         int size)
{
  DistanceTransformProcessor *processor = new DistanceTransformProcessor();
  processor->type = type;
  processor->size = size;
  emit processorCreated(processor);
}
