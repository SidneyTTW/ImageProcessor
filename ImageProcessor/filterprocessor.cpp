#include "filterprocessor.h"

#include "filterdialog.h"
#include "imagealgorithm.h"

FilterProcessor::FilterProcessor()
{
}

Area::AreaType FilterProcessor::acceptableAreaType() const
{
  return Area::TypeEmpty;
}

Area::AreaTypeFlag FilterProcessor::resultAreaType() const
{
  return Area::TypeEmpty;
}

Area FilterProcessor::resultArea() const
{
  return Area();
}

MyImage::ImageType FilterProcessor::acceptableType() const
{
  return (MyImage::ImageType)(MyImage::Colored |
                              MyImage::Gray |
                              MyImage::BlackAndWhite);
}

MyImage::ImageTypeFlag FilterProcessor::resultType() const
{
  return MyImage::Remain;
}

QImage *FilterProcessor::processImage(const QImage& image) const
{
  QImage *result = NULL;
  switch (_type)
  {
  case Midian:
    {
      ImageAlgorithm::AbstractFilter *filter =
          new ImageAlgorithm::MedianFilter(_width, _height);
      result = filter->filt(image);
      delete filter;
      break;
    }
  case Convolution:
    {
      ImageAlgorithm::AbstractFilter *filter =
          new ImageAlgorithm::ConvolutionFilter(_convolutionMatrix.data(),
                                                _width,
                                                _height,
                                                _convolutionDivisor,
                                                _convolutionOffset);
      result = filter->filt(image);
      delete filter;
      break;
    }
  }
  return result;
}

void FilterProcessor::processImage(QImage *image) const
{
  switch (_type)
  {
  case Midian:
    {
      ImageAlgorithm::AbstractFilter *filter =
          new ImageAlgorithm::MedianFilter(_width, _height);
      filter->filt(image);
      delete filter;
      break;
    }
  case Convolution:
    {
      ImageAlgorithm::AbstractFilter *filter =
          new ImageAlgorithm::ConvolutionFilter(_convolutionMatrix.data(),
                                                _width,
                                                _height,
                                                _convolutionDivisor,
                                                _convolutionOffset);
      filter->filt(image);
      delete filter;
      break;
    }
  }
}

QDialog *FilterProcessor::getOptionDialog(Area area,
                                          const MyImage& image)
{
  FilterDialog *result = new FilterDialog(image.getImage());
  connect(result,
          SIGNAL(confirmed(FilterProcessor::FilterType,
                           int,int,QVector<int>,int,int)),
          this,
          SLOT(confirm(FilterProcessor::FilterType,
                       int,int,QVector<int>,int,int)));
  return result;
}

QString FilterProcessor::name() const
{
  return "Filter";
}

QString FilterProcessor::toString() const
{
  QString result = tr("%1 %2 %3").
                   arg((int) _type).
                   arg(_width).
                   arg(_height);
  if (_type == Convolution)
  {
    result += tr(" %1 %2").arg(_convolutionDivisor).arg(_convolutionOffset);
    for (int i = 0;i < _convolutionMatrix.size();++i)
      result += tr(" %1").arg(_convolutionMatrix[i]);
  }
  return result;
}

AbstractImageProcessor *FilterProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() < 3)
    return NULL;
  FilterType type = (FilterType)list.takeFirst().toInt();
  int width = list.takeFirst().toInt();
  int height = list.takeFirst().toInt();
  QVector<int> matrix;
  int divisor = 1;
  int offset = 0;
  if (type == Convolution)
  {
    if (list.size() != width * height + 2)
      return NULL;
    divisor = list.takeFirst().toInt();
    offset = list.takeFirst().toInt();
    for (int i = 0;i < width * height;++i)
      matrix.push_back(list.takeFirst().toInt());
  }
  FilterProcessor *result = new FilterProcessor();
  result->_type = type;
  result->_width = width;
  result->_height = height;
  result->_convolutionMatrix = matrix;
  result->_convolutionDivisor = divisor;
  result->_convolutionOffset = offset;
  return result;
}

void FilterProcessor::confirm(FilterProcessor::FilterType type,
                              int width,
                              int height,
                              QVector<int> convolutionMatrix,
                              int convolutionDivisor,
                              int convolutionOffset)
{
  FilterProcessor *processor = new FilterProcessor();
  processor->_type = type;
  processor->_width = width;
  processor->_height = height;
  processor->_convolutionMatrix = convolutionMatrix;
  processor->_convolutionDivisor = convolutionDivisor;
  processor->_convolutionOffset = convolutionOffset;
  emit processorCreated(processor);
}
