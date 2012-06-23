#include "toblackandwhiteprocessor.h"

#include "imagealgorithm.h"
#include "toblackandwhitedialog.h"

ToBlackAndWhiteProcessor::ToBlackAndWhiteProcessor() :
    startColor(0)
{
}

Area::AreaType ToBlackAndWhiteProcessor::acceptableAreaType() const
{
  return Area::TypeAll;
}

Area::AreaTypeFlag ToBlackAndWhiteProcessor::resultAreaType() const
{
  return _area.getType();
}

Area ToBlackAndWhiteProcessor::resultArea() const
{
  return _area;
}

MyImage::ImageType ToBlackAndWhiteProcessor::acceptableType() const
{
  return (MyImage::ImageType)MyImage::Gray;
}

MyImage::ImageTypeFlag ToBlackAndWhiteProcessor::resultType() const
{
  return _area.getType() == Area::TypeEmpty ?
                            MyImage::BlackAndWhite :
                            MyImage::Gray;
}

QImage *ToBlackAndWhiteProcessor::processImage(const QImage& image) const
{
  QVector<int> thresholdCopy;
  if (type == ToBlackAndWhiteProcessor::OTSU)
    thresholdCopy.append(ImageAlgorithm::OTSU(image,
                                              ImageAlgorithm::Green,
                                              _area));
  else if (type == ToBlackAndWhiteProcessor::MaxEntropy)
    thresholdCopy.append(ImageAlgorithm::maxEntropy(image,
                                                    ImageAlgorithm::Green,
                                                    _area));
  else
    thresholdCopy = thresholds;
  QImage *result = ImageAlgorithm::convertToBlackAndWhite
                   (image, thresholdCopy, startColor, _area);
  return result;
}

void ToBlackAndWhiteProcessor::processImage(QImage *image) const
{
  QVector<int> thresholdCopy;
  if (type == ToBlackAndWhiteProcessor::OTSU)
    thresholdCopy.append(ImageAlgorithm::OTSU(*image,
                                              ImageAlgorithm::Green,
                                              _area));
  else if (type == ToBlackAndWhiteProcessor::MaxEntropy)
    thresholdCopy.append(ImageAlgorithm::maxEntropy(*image,
                                                    ImageAlgorithm::Green,
                                                    _area));
  else
    thresholdCopy = thresholds;
  ImageAlgorithm::convertToBlackAndWhite(image, thresholdCopy, startColor, _area);
}

QDialog *ToBlackAndWhiteProcessor::getOptionDialog(Area area,
                                                   const MyImage& image)
{
  ToBlackAndWhiteDialog *result = new ToBlackAndWhiteDialog(image, area);
  connect(result,
          SIGNAL(confirm(ToBlackAndWhiteProcessor::ThresholdType,
                         int,
                         QVector<int>,
                         Area)),
          this,
          SLOT(confirm(ToBlackAndWhiteProcessor::ThresholdType,
                       int,
                       QVector<int>,
                       Area)));
  return result;
}

QString ToBlackAndWhiteProcessor::name() const
{
  return "Black And White";
}

QString ToBlackAndWhiteProcessor::toString() const
{
  QString result = tr("%1 %2 %3").
                   arg((int) type).
                   arg(startColor).
                   arg(_area.toString());
  for (int i = 0;i < thresholds.size();++i)
    result += tr(" %1").arg(thresholds[i]);
  return result;
}

AbstractImageProcessor *ToBlackAndWhiteProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() < 3)
    return NULL;
  ToBlackAndWhiteProcessor *result = new ToBlackAndWhiteProcessor();
  result->type = (ThresholdType) list.takeFirst().toInt();
  result->startColor = list.takeFirst().toInt();
  result->_area = Area::fromString(list.takeFirst());
  while (!list.isEmpty())
      result->thresholds.push_back(list.takeFirst().toInt());
  return result;
}

QString ToBlackAndWhiteProcessor::description() const
{
  return "Turn the image into black and white";
}

QString ToBlackAndWhiteProcessor::iconPath() const
{
  return Resource::iconBlackAndWhite;
}

void ToBlackAndWhiteProcessor::confirm
    (ToBlackAndWhiteProcessor::ThresholdType type,
     int startColor,
     QVector<int> thresholds,
     Area area)
{
  ToBlackAndWhiteProcessor *processor = new ToBlackAndWhiteProcessor();
  processor->type = type;
  processor->startColor = startColor;
  processor->thresholds = thresholds;
  processor->_area = area;
  emit processorCreated(processor);
}
