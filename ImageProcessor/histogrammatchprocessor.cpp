#include "histogrammatchprocessor.h"

#include "histogrammatchdialog.h"
#include "imagealgorithm.h"

HistogramMatchProcessor::HistogramMatchProcessor()
{
}

Area::AreaType HistogramMatchProcessor::acceptableAreaType() const
{
  return Area::TypeEmpty;
}

Area::AreaTypeFlag HistogramMatchProcessor::resultAreaType() const
{
  return Area::TypeEmpty;
}

Area HistogramMatchProcessor::resultArea() const
{
  return Area();
}

MyImage::ImageType HistogramMatchProcessor::acceptableType() const
{
  return MyImage::Gray;
}

MyImage::ImageTypeFlag HistogramMatchProcessor::resultType() const
{
  return MyImage::Gray;
}

QImage *HistogramMatchProcessor::processImage(const QImage& image) const
{
  QImage another = QImage(_path).convertToFormat(QImage::Format_ARGB32);
  BasicStatistic statistic1 = ImageAlgorithm::getStatistic(image, ImageAlgorithm::Green);
  BasicStatistic statistic2 = ImageAlgorithm::getStatistic(another, ImageAlgorithm::Green);
  int mapR[MAX_COLOR_VALUE + 1];
  int mapG[MAX_COLOR_VALUE + 1];
  int mapB[MAX_COLOR_VALUE + 1];
  calculateRGBMap(statistic1, statistic2, mapR, mapG, mapB);
  QImage *result = ImageAlgorithm::changeRGBWithMap
                   (image, mapR, mapG, mapB,
                    (ImageAlgorithm::RGBAField)(ImageAlgorithm::Field_R |
                                                ImageAlgorithm::Field_G |
                                                ImageAlgorithm::Field_B));
  return result;
}

void HistogramMatchProcessor::processImage(QImage *image) const
{
  QImage another = QImage(_path).convertToFormat(QImage::Format_ARGB32);
  BasicStatistic statistic1 = ImageAlgorithm::getStatistic(*image, ImageAlgorithm::Green);
  BasicStatistic statistic2 = ImageAlgorithm::getStatistic(another, ImageAlgorithm::Green);
  int mapR[MAX_COLOR_VALUE + 1];
  int mapG[MAX_COLOR_VALUE + 1];
  int mapB[MAX_COLOR_VALUE + 1];
  calculateRGBMap(statistic1, statistic2, mapR, mapG, mapB);
  ImageAlgorithm::changeRGBWithMap
      (image, mapR, mapG, mapB,
       (ImageAlgorithm::RGBAField)(ImageAlgorithm::Field_R |
                                   ImageAlgorithm::Field_G |
                                   ImageAlgorithm::Field_B));
}

QDialog *HistogramMatchProcessor::getOptionDialog(Area area, const MyImage& image)
{
  HistogramMatchDialog *result = new HistogramMatchDialog(image.getImage());
  connect(result,
          SIGNAL(confirm(QString)),
          this,
          SLOT(confirm(QString)));
  return result;
}

void HistogramMatchProcessor::calculateRGBMap(const BasicStatistic& statistic1,
                                              const BasicStatistic& statistic2,
                                              int mapR[MAX_COLOR_VALUE + 1],
                                              int mapG[MAX_COLOR_VALUE + 1],
                                              int mapB[MAX_COLOR_VALUE + 1])
{
  double rates[MAX_COLOR_VALUE + 1];
  int sum = 0;
  for (int i = 0;i <= MAX_COLOR_VALUE;++i)
  {
    sum += statistic2.counts[BasicStatistic::Gray][i];
    rates[i] = 1.0 * sum / (statistic2._width * statistic2._height);
  }
  int index = 0;
  sum = 0;
  for (int i = 0;i <= MAX_COLOR_VALUE;++i)
  {
    sum += statistic1.counts[BasicStatistic::Gray][i];
    while (index < MAX_COLOR_VALUE &&
           1.0 * sum / (statistic1._width * statistic1._height) > rates[index])
      ++index;
    mapR[i] = index;
    mapG[i] = index;
    mapB[i] = index;
  }
}

QString HistogramMatchProcessor::name() const
{
  return "Histogram Match";
}

QString HistogramMatchProcessor::toString() const
{
  QString path = _path;
  path.replace(QChar(' '), QChar('*'));
  QString result = tr("%1").arg(path);
  return result;
}

AbstractImageProcessor *HistogramMatchProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() != 1)
    return NULL;
  HistogramMatchProcessor *result = new HistogramMatchProcessor();
  result->_path = list.takeFirst().replace(QChar('*'), QChar(' '));;
  return result;
}

QString HistogramMatchProcessor::description() const
{
  return "Do histogram match between two images";
}

QString HistogramMatchProcessor::iconPath() const
{
  return Resource::iconHistogramMatch;
}

void HistogramMatchProcessor::confirm(QString path)
{
  HistogramMatchProcessor *processor = new HistogramMatchProcessor();
  processor->_path = path;
  emit processorCreated(processor);
}

