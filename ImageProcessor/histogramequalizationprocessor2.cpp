#include "histogramequalizationprocessor2.h"

#include "histogramequalizationprocessor.h"
#include "imagealgorithm.h"

HistogramEqualizationProcessor2::HistogramEqualizationProcessor2()
{
}

Area::AreaType HistogramEqualizationProcessor2::acceptableAreaType() const
{
  return Area::TypeAll;
}

Area::AreaTypeFlag HistogramEqualizationProcessor2::resultAreaType() const
{
  return _area.getType();
}

Area HistogramEqualizationProcessor2::resultArea() const
{
  return _area;
}

MyImage::ImageType HistogramEqualizationProcessor2::acceptableType() const
{
  return (MyImage::ImageType)MyImage::Gray;
}

MyImage::ImageTypeFlag HistogramEqualizationProcessor2::resultType() const
{
  return MyImage::Gray;
}

QImage *HistogramEqualizationProcessor2::processImage(const QImage& image) const
{
  BasicStatistic statistic =
      ImageAlgorithm::getStatistic(image, ImageAlgorithm::Green, _area);
  int mapR[MAX_COLOR_VALUE + 1];
  int mapG[MAX_COLOR_VALUE + 1];
  int mapB[MAX_COLOR_VALUE + 1];
  ImageAlgorithm::RGBAField tunel = (ImageAlgorithm::RGBAField)(ImageAlgorithm::Field_R |
                                                                ImageAlgorithm::Field_G |
                                                                ImageAlgorithm::Field_B);
  HistogramEqualizationProcessor::calculateRGBMap(statistic, mapR, mapG, mapB, tunel);
  QImage *result = ImageAlgorithm::changeRGBWithMap(image, mapR, mapG, mapB, tunel, _area);
  return result;
}

void HistogramEqualizationProcessor2::processImage(QImage *image) const
{
  BasicStatistic statistic =
      ImageAlgorithm::getStatistic(*image, ImageAlgorithm::Green, _area);
  int mapR[MAX_COLOR_VALUE + 1];
  int mapG[MAX_COLOR_VALUE + 1];
  int mapB[MAX_COLOR_VALUE + 1];
  ImageAlgorithm::RGBAField tunel = (ImageAlgorithm::RGBAField)(ImageAlgorithm::Field_R |
                                                                ImageAlgorithm::Field_G |
                                                                ImageAlgorithm::Field_B);
  HistogramEqualizationProcessor::calculateRGBMap(statistic, mapR, mapG, mapB, tunel);
  ImageAlgorithm::changeRGBWithMap(image, mapR, mapG, mapB, tunel, _area);
}

QDialog *HistogramEqualizationProcessor2::getOptionDialog(Area area,
                                                          const MyImage& image)
{
  HistogramEqualizationProcessor2 *processor = new HistogramEqualizationProcessor2();
  processor->_area = area;
  emit processorCreated(processor);
  return NULL;
}

QString HistogramEqualizationProcessor2::name() const
{
  return "Histogram Equalization2";
}

QString HistogramEqualizationProcessor2::toString() const
{
  return _area.toString();
}

AbstractImageProcessor *HistogramEqualizationProcessor2::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() != 1)
    return NULL;
  HistogramEqualizationProcessor2 *result = new HistogramEqualizationProcessor2();
  result->_area = Area::fromString(list.takeFirst());
  return result;
}

QString HistogramEqualizationProcessor2::description() const
{
  return "Histogram Equalization";
}

QString HistogramEqualizationProcessor2::iconPath() const
{
  return Resource::iconHistogramEqualization;
}
