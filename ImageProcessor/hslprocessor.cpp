#include "hslprocessor.h"

#include <QStringList>
#include "hsldialog.h"

HSLProcessor::HSLProcessor()
{
}

Area::AreaType HSLProcessor::acceptableAreaType() const
{
  return Area::TypeEmpty;
}

Area::AreaTypeFlag HSLProcessor::resultAreaType() const
{
  return Area::TypeEmpty;
}

Area HSLProcessor::resultArea() const
{
  return Area();
}

MyImage::ImageType HSLProcessor::acceptableType() const
{
  return MyImage::Colored;
}

MyImage::ImageTypeFlag HSLProcessor::resultType() const
{
  return MyImage::Colored;
}

QImage *HSLProcessor::processImage(const QImage& image) const
{
  QImage *result = ImageAlgorithm::changeHSL(image, _h, _s, _l);
  return result;
}

void HSLProcessor::processImage(QImage *image) const
{
  ImageAlgorithm::changeHSL(image, _h, _s, _l);
}

QDialog *HSLProcessor::getOptionDialog(Area area, const MyImage& image)
{
  HSLDialog *result = new HSLDialog(image.getImage());
  connect(result,
          SIGNAL(confirm(double,double,double)),
          this,
          SLOT(confirm(double,double,double)));
  return result;
}

QString HSLProcessor::name() const
{
  return "HSL";
}

QString HSLProcessor::toString() const
{
  QString result = tr("%1 %2 %3").
                   arg(QString::number(_h)).
                   arg(QString::number(_s)).
                   arg(QString::number(_l));
  return result;
}

AbstractImageProcessor *HSLProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() != 3)
    return NULL;
  HSLProcessor *result = new HSLProcessor();
  result->_h = list.takeFirst().toDouble();
  result->_s = list.takeFirst().toDouble();
  result->_l = list.takeFirst().toDouble();
  return result;
}

QString HSLProcessor::description() const
{
  return "Change the HSL";
}

QString HSLProcessor::iconPath() const
{
  return "";
}

void HSLProcessor::confirm(double h, double s, double l)
{
  HSLProcessor *processor = new HSLProcessor();
  processor->_h = h;
  processor->_s = s;
  processor->_l = l;
  emit processorCreated(processor);
}
