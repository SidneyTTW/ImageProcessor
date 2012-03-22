#include "resizeprocessor.h"

#include <QStringList>
#include "resizedialog.h"

ResizeProcessor::ResizeProcessor()
{
}

Area::AreaType ResizeProcessor::acceptableAreaType() const
{
  return Area::TypeEmpty;
}

Area::AreaTypeFlag ResizeProcessor::resultAreaType() const
{
  return Area::TypeEmpty;
}

Area ResizeProcessor::resultArea() const
{
  return Area();
}

MyImage::ImageType ResizeProcessor::acceptableType() const
{
  return (MyImage::ImageType)(MyImage::Colored |
                              MyImage::Gray |
                              MyImage::BlackAndWhite);
}

MyImage::ImageTypeFlag ResizeProcessor::resultType() const
{
  return MyImage::Remain;
}

QImage *ResizeProcessor::processImage(const QImage& image) const
{
  int width;
  int height;
  switch (_sizeType)
  {
  case Absolute:
    width = _width;
    height = _height;
    break;
  case Relative:
    width = _width * image.width();
    height = _height * image.height();
    break;
  }
  QImage *result = ImageAlgorithm::resize
                   (image, width, height, _algorithmType);
  return result;
}

void ResizeProcessor::processImage(QImage *image) const
{
  int width;
  int height;
  switch (_sizeType)
  {
  case Absolute:
    width = _width;
    height = _height;
    break;
  case Relative:
    width = _width * image->width();
    height = _height * image->height();
    break;
  }
  ImageAlgorithm::resize(image, width, height, _algorithmType);
}

QDialog *ResizeProcessor::getOptionDialog(Area area, const MyImage& image)
{
  ResizeDialog *result = new ResizeDialog(image.getImage());
  connect(result,
          SIGNAL(confirm(ResizeProcessor::SizeType,
                         ImageAlgorithm::ResizeAlgorithmType,
                         double,
                         double)),
          this,
          SLOT(confirm(ResizeProcessor::SizeType,
                       ImageAlgorithm::ResizeAlgorithmType,
                       double,
                       double)));
  return result;
}

QString ResizeProcessor::name() const
{
  return "Resize";
}

QString ResizeProcessor::toString() const
{
  QString result = tr("%1 %2 %3 %4").
                   arg((int) _sizeType).
                   arg((int) _algorithmType).
                   arg(QString::number(_width)).
                   arg(QString::number(_height));
  return result;
}

AbstractImageProcessor *ResizeProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() != 4)
    return NULL;
  ResizeProcessor *result = new ResizeProcessor();
  result->_sizeType = (SizeType) list.takeFirst().toInt();
  result->_algorithmType = (ImageAlgorithm::ResizeAlgorithmType)
                           list.takeFirst().toInt();
  result->_width = list.takeFirst().toDouble();
  result->_height = list.takeFirst().toDouble();
  return result;
}

void ResizeProcessor::confirm
    (ResizeProcessor::SizeType sizeType,
     ImageAlgorithm::ResizeAlgorithmType algorithmType,
     double width,
     double height)
{
  ResizeProcessor *processor = new ResizeProcessor();
  processor->_sizeType = sizeType;
  processor->_algorithmType = algorithmType;
  processor->_width = width;
  processor->_height = height;
  emit processorCreated(processor);
}
