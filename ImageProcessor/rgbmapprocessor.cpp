#include "rgbmapprocessor.h"

#include "rgbmapdialog.h"
#include "imagealgorithm.h"

RGBMapProcessor::RGBMapProcessor()
{
}

Area::AreaType RGBMapProcessor::acceptableAreaType() const
{
  return Area::TypeAll;
}

Area::AreaTypeFlag RGBMapProcessor::resultAreaType() const
{
  return _area.getType();
}

Area RGBMapProcessor::resultArea() const
{
  return _area;
}

MyImage::ImageType RGBMapProcessor::acceptableType() const
{
  return (MyImage::ImageType) (MyImage::Colored |
                               MyImage::Gray |
                               MyImage::BlackAndWhite);
}

MyImage::ImageTypeFlag RGBMapProcessor::resultType() const
{
  return MyImage::Colored;
}

QImage *RGBMapProcessor::processImage(const QImage& image) const
{
  QImage *result = ImageAlgorithm::changeRGBWithMap
                   (image, map[0], map[1], map[2],
                    (ImageAlgorithm::RGBAField)(ImageAlgorithm::Field_R |
                                                ImageAlgorithm::Field_G |
                                                ImageAlgorithm::Field_B));
  return result;
}

void RGBMapProcessor::processImage(QImage *image) const
{
  ImageAlgorithm::changeRGBWithMap
                     (image, map[0], map[1], map[2],
                      (ImageAlgorithm::RGBAField)(ImageAlgorithm::Field_R |
                                                  ImageAlgorithm::Field_G |
                                                  ImageAlgorithm::Field_B));
}

QDialog *RGBMapProcessor::getOptionDialog(Area area, const MyImage& image)
{
  RGBMapDialog *result = new RGBMapDialog(image.getImage(), area);
  connect(result,
          SIGNAL(confirmed(int*,Area)),
          this,
          SLOT(confirm(int*,Area)));
  return result;
}

QString RGBMapProcessor::name() const
{
  return "RGB Map";
}

QString RGBMapProcessor::toString() const
{
  QString path;
  for (int i = 0; i < 3;++i)
    for (int j = 0;j <= MAX_COLOR_VALUE;++j)
      path += tr("%1 ").arg(map[i][j]);
  path += _area.toString();
  return path;
}

AbstractImageProcessor *RGBMapProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() != 3 * (MAX_COLOR_VALUE + 1) + 1)
    return NULL;
  RGBMapProcessor *result = new RGBMapProcessor();
  for (int i = 0; i < 3;++i)
    for (int j = 0;j <= MAX_COLOR_VALUE;++j)
      result->map[i][j] = list.takeFirst().toInt();
  result->_area = Area::fromString(list.takeFirst());
  return result;
}

QString RGBMapProcessor::description() const
{
  return "Do rgb map";
}

QString RGBMapProcessor::iconPath() const
{
  return Resource::iconRGBMap;
}

void RGBMapProcessor::confirm(int *map, Area area)
{
  RGBMapProcessor *processor = new RGBMapProcessor();
  for (int i = 0; i < 3;++i)
    for (int j = 0;j <= MAX_COLOR_VALUE;++j)
      processor->map[i][j] = map[i * (MAX_COLOR_VALUE + 1) + j];
  processor->_area = area;
  emit processorCreated(processor);
}
