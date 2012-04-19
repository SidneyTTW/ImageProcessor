#include "pointprocessor.h"

#include "imagealgorithm.h"
#include "pointdialog.h"

PointProcessor::PointProcessor()
{
}

Area::AreaType PointProcessor::acceptableAreaType() const
{
  return Area::TypeAll;
}

Area::AreaTypeFlag PointProcessor::resultAreaType() const
{
  return _area.getType();
}

Area PointProcessor::resultArea() const
{
  return _area;
}

MyImage::ImageType PointProcessor::acceptableType() const
{
  return MyImage::Gray;
}

MyImage::ImageTypeFlag PointProcessor::resultType() const
{
  return MyImage::Gray;
}

QImage *PointProcessor::processImage(const QImage& image) const
{
  int mapR[MAX_COLOR_VALUE + 1];
  int mapG[MAX_COLOR_VALUE + 1];
  int mapB[MAX_COLOR_VALUE + 1];
  calculateRGBMap(_type, _para1, _para2, mapR, mapG, mapB);
  QImage *result = ImageAlgorithm::changeRGBWithMap
      (image, mapR, mapG, mapB,
       (ImageAlgorithm::RGBAField)(ImageAlgorithm::Field_R |
                                   ImageAlgorithm::Field_G |
                                   ImageAlgorithm::Field_B),
       _area);
  return result;
}

void PointProcessor::processImage(QImage *image) const
{
  int mapR[MAX_COLOR_VALUE + 1];
  int mapG[MAX_COLOR_VALUE + 1];
  int mapB[MAX_COLOR_VALUE + 1];
  calculateRGBMap(_type, _para1, _para2, mapR, mapG, mapB);
  ImageAlgorithm::changeRGBWithMap
      (image, mapR, mapG, mapB,
       (ImageAlgorithm::RGBAField)(ImageAlgorithm::Field_R |
                                   ImageAlgorithm::Field_G |
                                   ImageAlgorithm::Field_B),
       _area);
}

QDialog *PointProcessor::getOptionDialog(Area area,
                                         const MyImage& image)
{
  PointDialog *result = new PointDialog(image.getImage(), area);
  connect(result,
          SIGNAL(confirm(Area,PointProcessor::ProcessorType,double,double)),
          this,
          SLOT(confirm(Area,PointProcessor::ProcessorType,double,double)));
  return result;
}

QString PointProcessor::name() const
{
  return "Point Operator";
}

QString PointProcessor::toString() const
{
  QString result = tr("%1 %2 %3 %4").
                   arg((int) _type).
                   arg(QString::number(_para1)).
                   arg(QString::number(_para2)).
                   arg(_area.toString());
  return result;
}

AbstractImageProcessor *PointProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() != 4)
    return NULL;
  PointProcessor *result = new PointProcessor();
  result->_type = (ProcessorType) list.takeFirst().toInt();
  result->_para1 = list.takeFirst().toDouble();
  result->_para2 = list.takeFirst().toDouble();
  result->_area = Area::fromString(list.takeFirst());
  return result;
}

QString PointProcessor::description() const
{
  return "Use point processor to process the image";
}

QString PointProcessor::iconPath() const
{
  return Resource::iconPointProcessor;
}

void PointProcessor::confirm(Area area,
                             PointProcessor::ProcessorType type,
                             double para1,
                             double para2)
{
  PointProcessor *processor = new PointProcessor();
  processor->_area = area;
  processor->_type = type;
  processor->_para1 = para1;
  processor->_para2 = para2;
  emit processorCreated(processor);
}

void PointProcessor::calculateRGBMap
    (ProcessorType type,
     double para1,
     double para2,
     int mapR[MAX_COLOR_VALUE + 1],
     int mapG[MAX_COLOR_VALUE + 1],
     int mapB[MAX_COLOR_VALUE + 1])
{
  for (int i = 0;i <= MAX_COLOR_VALUE;++i)
  {
    int value;
    switch (type)
    {
    case Linear:
      value = qBound(0, (int)(i * para1 + para2), MAX_COLOR_VALUE);
      break;
    case NonLinear:
      value = qBound(0,
                     (int)(i + i * para1 * (para2 - i)),
                     MAX_COLOR_VALUE);
      break;
    default:
      value = i;
      break;
    }
    mapR[i] = value;
    mapG[i] = value;
    mapB[i] = value;
  }
}
