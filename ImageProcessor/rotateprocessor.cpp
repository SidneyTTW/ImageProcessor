#include "rotateprocessor.h"

#include <QStringList>
#include "rotatedialog.h"

RotateProcessor::RotateProcessor()
{
}

Area::AreaType RotateProcessor::acceptableAreaType() const
{
  return Area::TypeAll;
}

Area::AreaTypeFlag RotateProcessor::resultAreaType() const
{
  return _area.getType();
}

Area RotateProcessor::resultArea() const
{
  return _area;
}

MyImage::ImageType RotateProcessor::acceptableType() const
{
  return (MyImage::ImageType)(MyImage::Colored |
                              MyImage::Gray |
                              MyImage::BlackAndWhite);
}

MyImage::ImageTypeFlag RotateProcessor::resultType() const
{
  return MyImage::Remain;
}

QImage *RotateProcessor::processImage(const QImage& image) const
{
  QImage *result = ImageAlgorithm::rotate(image, _angle, _algorithmType, _area);
  return result;
}

void RotateProcessor::processImage(QImage *image) const
{
  ImageAlgorithm::rotate(image, _angle, _algorithmType, _area);
}

QDialog *RotateProcessor::getOptionDialog(Area area, const MyImage& image)
{
  RotateDialog *result = new RotateDialog(image.getImage(), area);
  connect(result,
          SIGNAL(confirm(Area,ImageAlgorithm::GeometricAlgorithmType,double)),
          this,
          SLOT(confirm(Area,ImageAlgorithm::GeometricAlgorithmType,double)));
  return result;
}

QString RotateProcessor::name() const
{
  return "Rotate";
}

QString RotateProcessor::toString() const
{
  QString result = tr("%1 %2 %3").
                   arg((int) _algorithmType).
                   arg(QString::number(_angle)).
                   arg(_area.toString());
  return result;
}

AbstractImageProcessor *RotateProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() != 3)
    return NULL;
  RotateProcessor *result = new RotateProcessor();
  result->_algorithmType = (ImageAlgorithm::GeometricAlgorithmType)
                           list.takeFirst().toInt();
  result->_angle = list.takeFirst().toDouble();
  result->_area = Area::fromString(list.takeFirst());
  return result;
}

QString RotateProcessor::description() const
{
  return "Rotate whole image or selected area";
}

QString RotateProcessor::iconPath() const
{
  return Resource::iconRotate;
}

void RotateProcessor::confirm
    (Area area,
     ImageAlgorithm::GeometricAlgorithmType algorithmType,
     double angle)
{
  RotateProcessor *processor = new RotateProcessor();
  processor->_algorithmType = algorithmType;
  processor->_angle = angle;
  processor->_area = area;
  emit processorCreated(processor);
}
