#include "filterprocessor.h"

#include "filterdialog.h"
#include "imagealgorithm.h"

FilterProcessor::FilterProcessor()
{
}

Area::AreaType FilterProcessor::acceptableAreaType() const
{
  return Area::TypeAll;
}

Area::AreaTypeFlag FilterProcessor::resultAreaType() const
{
  return _area.getType();
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
      ImageAlgorithm::MedianFilter *filter =
          new ImageAlgorithm::MedianFilter(_width, _height);
      result = ImageAlgorithm::filtImage<ImageAlgorithm::MedianFilter>(image, _area, filter);
      delete filter;
      break;
    }
  case Convolution:
    {
      ImageAlgorithm::ConvolutionFilter *filter =
          new ImageAlgorithm::ConvolutionFilter(_convolutionMatrix.data(),
                                                _width,
                                                _height,
                                                _convolutionDivisor,
                                                _convolutionOffset);
      result = ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(image, _area, filter);
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
      ImageAlgorithm::MedianFilter *filter =
          new ImageAlgorithm::MedianFilter(_width, _height);
      ImageAlgorithm::filtImage<ImageAlgorithm::MedianFilter>(image, _area, filter);
      delete filter;
      break;
    }
  case Convolution:
    {
      ImageAlgorithm::ConvolutionFilter *filter =
          new ImageAlgorithm::ConvolutionFilter(_convolutionMatrix.data(),
                                                _width,
                                                _height,
                                                _convolutionDivisor,
                                                _convolutionOffset);
      ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(image, _area, filter);
      delete filter;
      break;
    }
  }
}

QDialog *FilterProcessor::getOptionDialog(Area area,
                                          const MyImage& image)
{
  FilterDialog *result = new FilterDialog(image.getImage(), area);
  connect(result,
          SIGNAL(confirmed(FilterProcessor::FilterType,
                           int,int,Area,QVector<int>,int,int)),
          this,
          SLOT(confirm(FilterProcessor::FilterType,
                       int,int,Area,QVector<int>,int,int)));
  return result;
}

QString FilterProcessor::name() const
{
  return "Filter";
}

QString FilterProcessor::toString() const
{
  QString result = tr("%1 %2 %3 %4").
                   arg((int) _type).
                   arg(_width).
                   arg(_height).
                   arg(_area.toString());
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
  if (list.size() < 4)
    return NULL;
  FilterType type = (FilterType)list.takeFirst().toInt();
  int width = list.takeFirst().toInt();
  int height = list.takeFirst().toInt();
  Area area = Area::fromString(list.takeFirst());
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
  result->_area = area;
  return result;
}

void FilterProcessor::confirm(FilterProcessor::FilterType type,
                              int width,
                              int height,
                              Area area,
                              QVector<int> convolutionMatrix,
                              int convolutionDivisor,
                              int convolutionOffset)
{
  FilterProcessor *processor = new FilterProcessor();
  processor->_type = type;
  processor->_width = width;
  processor->_height = height;
  processor->_area = area;
  processor->_convolutionMatrix = convolutionMatrix;
  processor->_convolutionDivisor = convolutionDivisor;
  processor->_convolutionOffset = convolutionOffset;
  emit processorCreated(processor);
}
