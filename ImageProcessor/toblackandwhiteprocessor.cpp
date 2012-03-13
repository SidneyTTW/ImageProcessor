#include "toblackandwhiteprocessor.h"

#include "imagealgorithm.h"
#include "toblackandwhitedialog.h"

ToBlackAndWhiteProcessor::ToBlackAndWhiteProcessor() :
    startColor(0)
{
}

Area::AreaType ToBlackAndWhiteProcessor::acceptableAreaType() const
{
  return Area::TypeEmpty;
}

Area::AreaTypeFlag ToBlackAndWhiteProcessor::resultAreaType() const
{
  return Area::TypeEmpty;
}

Area ToBlackAndWhiteProcessor::resultArea() const
{
  return Area();
}

MyImage::ImageType ToBlackAndWhiteProcessor::acceptableType() const
{
  return (MyImage::ImageType)MyImage::Gray;
}

MyImage::ImageTypeFlag ToBlackAndWhiteProcessor::resultType() const
{
  return MyImage::BlackAndWhite;
}

MyImage *ToBlackAndWhiteProcessor::processImage(const MyImage& image) const
{
  QVector<int> thresholdCopy;
  if (type == ToBlackAndWhiteProcessor::OTSU)
    thresholdCopy.append(ImageAlgorithm::OTSU(image.getImage(),
                                              ImageAlgorithm::Green));
  else if (type == ToBlackAndWhiteProcessor::MaxEntropy)
    thresholdCopy.append(ImageAlgorithm::maxEntropy(image.getImage(),
                                                    ImageAlgorithm::Green));
  else
    thresholdCopy = thresholds;
  QImage *resultImage = ImageAlgorithm::convertToBlackAndWhite
                        (image.getImage(), thresholdCopy, startColor);
  MyImage *result = new MyImage(*resultImage, MyImage::BlackAndWhite);
  delete resultImage;
  return result;
}

QDialog *ToBlackAndWhiteProcessor::getOptionDialog(Area area,
                                                   const MyImage& image)
{
  ToBlackAndWhiteDialog *result = new ToBlackAndWhiteDialog(image);
  connect(result,
          SIGNAL(confirm(ToBlackAndWhiteProcessor::ThresholdType,
                         int,
                         QVector<int>)),
          this,
          SLOT(confirm(ToBlackAndWhiteProcessor::ThresholdType,
                       int,
                       QVector<int>)));
  return result;
}

QString ToBlackAndWhiteProcessor::name() const
{
  return "Black And White";
}

QString ToBlackAndWhiteProcessor::toString() const
{
  QString result = tr("%1 %2").arg((int) type).arg(startColor);
  for (int i = 0;i < thresholds.size();++i)
    result += tr(" %1").arg(thresholds[i]);
  return result;
}

AbstractImageProcessor *ToBlackAndWhiteProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() < 2)
    return NULL;
  ToBlackAndWhiteProcessor *result = new ToBlackAndWhiteProcessor();
  result->type = (ThresholdType) list.takeFirst().toInt();
  result->startColor = list.takeFirst().toInt();
  while (!list.isEmpty())
      result->thresholds.push_back(list.takeFirst().toInt());
  return result;
}

void ToBlackAndWhiteProcessor::confirm
    (ToBlackAndWhiteProcessor::ThresholdType type,
     int startColor,
     QVector<int> thresholds)
{
  ToBlackAndWhiteProcessor *processor = new ToBlackAndWhiteProcessor();
  processor->type = type;
  processor->startColor = startColor;
  processor->thresholds = thresholds;
  emit processorCreated(processor);
}