#include "histogramequalizationprocessor.h"

#include "histogramequalizationoptionwidget.h"

HistogramEqualizationProcessor::HistogramEqualizationProcessor() :
    _tunel(ImageAlgorithm::Field_R |
           ImageAlgorithm::Field_G |
           ImageAlgorithm::Field_B),
    _optionWidget(NULL)
{
}

MyImage::ImageTypeFlag HistogramEqualizationProcessor::resultType() const
{
  return (MyImage::ImageTypeFlag)MyImage::Remain;
}

QImage *HistogramEqualizationProcessor::processImage(const QImage& image) const
{
  BasicStatistic statistic =
      ImageAlgorithm::getStatistic(image, ImageAlgorithm::Green);
  int mapR[MAX_COLOR_VALUE + 1];
  int mapG[MAX_COLOR_VALUE + 1];
  int mapB[MAX_COLOR_VALUE + 1];
  calculateRGBMap(statistic, mapR, mapG, mapB);
  QImage *result = ImageAlgorithm::changeRGBWithMap(image,
                                                    mapR,
                                                    mapG,
                                                    mapB,
                                                    _tunel);
  return result;
}

void HistogramEqualizationProcessor::processImage(QImage *image) const
{
  BasicStatistic statistic =
      ImageAlgorithm::getStatistic(*image, ImageAlgorithm::Green);
  int mapR[MAX_COLOR_VALUE];
  int mapG[MAX_COLOR_VALUE];
  int mapB[MAX_COLOR_VALUE];
  calculateRGBMap(statistic, mapR, mapG, mapB);
  ImageAlgorithm::changeRGBWithMap(image, mapR, mapG, mapB, _tunel);
}

QWidget *HistogramEqualizationProcessor::optionWidget()
{
  if (_optionWidget == NULL)
  {
    _optionWidget = new HistogramEqualizationOptionWidget(_tunel);
    connect(_optionWidget,
            SIGNAL(changeToTunel(ImageAlgorithm::RGBAField)),
            this,
            SLOT(changeToTunel(ImageAlgorithm::RGBAField)));
    connect(_optionWidget, SIGNAL(confirm()), this, SLOT(confirm()));
  }
  return _optionWidget;
}

MyImage HistogramEqualizationProcessor::preProcessImage(const MyImage& image) const
{
  QImage *resultImage = processImage(image.getImage());
  MyImage result(*resultImage, MyImage::Gray);
  delete resultImage;
  return result;
}

bool HistogramEqualizationProcessor::cancelWhenNewOneIsCreated() const
{
  return true;
}

void HistogramEqualizationProcessor::interrupt()
{
}

void HistogramEqualizationProcessor::calculateRGBMap
    (const BasicStatistic& statistic,
     int mapR[MAX_COLOR_VALUE],
     int mapG[MAX_COLOR_VALUE],
     int mapB[MAX_COLOR_VALUE]) const
{
  int sumR = 0, sumG = 0, sumB = 0;
  int points = statistic._width * statistic._height;
  for (int i = 0;i <= MAX_COLOR_VALUE;++i)
  {
    if (_tunel.testFlag(ImageAlgorithm::Field_R))
    {
      sumR += statistic.counts[BasicStatistic::Red][i];
      mapR[i] = MAX_COLOR_VALUE * sumR / points;
    }
    if (_tunel.testFlag(ImageAlgorithm::Field_G))
    {
      sumG += statistic.counts[BasicStatistic::Green][i];
      mapG[i] = MAX_COLOR_VALUE * sumG / points;
    }
    if (_tunel.testFlag(ImageAlgorithm::Field_B))
    {
      sumB += statistic.counts[BasicStatistic::Blue][i];
      mapB[i] = MAX_COLOR_VALUE * sumB / points;
    }
  }
}

QString HistogramEqualizationProcessor::name() const
{
  return "HistogramEqualization";
}

QString HistogramEqualizationProcessor::toString() const
{
  return tr("%1").arg((int)_tunel);
}

AbstractImageProcessor *HistogramEqualizationProcessor::fromString
    (const QString& str) const
{
  HistogramEqualizationProcessor *result = new HistogramEqualizationProcessor();
  result->_tunel = (ImageAlgorithm::RGBAField) str.toInt();
  return result;
}

QString HistogramEqualizationProcessor::description() const
{
  return "Histogram Equalization";
}

QString HistogramEqualizationProcessor::iconPath() const
{
  return Resource::iconHistogramEqualization;
}

void HistogramEqualizationProcessor::confirm()
{
  HistogramEqualizationProcessor *newProcessor =
      new HistogramEqualizationProcessor();
  newProcessor->_tunel = _tunel;
  emit processorCreated(newProcessor);
}

void HistogramEqualizationProcessor::changeToTunel
    (ImageAlgorithm::RGBAField tunel)
{
  _tunel = tunel;
  emit optionChanged(this);
}
