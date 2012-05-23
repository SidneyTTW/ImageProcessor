#include "dilationerosionprocessor.h"

#include "dilationerosiondialog.h"
#include "imagealgorithm.h"

DilationErosionProcessor::DilationErosionProcessor()
{
}

Area::AreaType DilationErosionProcessor::acceptableAreaType() const
{
  return Area::TypeAll;
}

Area::AreaTypeFlag DilationErosionProcessor::resultAreaType() const
{
  return _area.getType();
}

Area DilationErosionProcessor::resultArea() const
{
  return Area();
}

MyImage::ImageType DilationErosionProcessor::acceptableType() const
{
  return (MyImage::ImageType) (MyImage::BlackAndWhite |
                               MyImage::Gray);
}

MyImage::ImageTypeFlag DilationErosionProcessor::resultType() const
{
  return MyImage::Remain;
}

QImage *DilationErosionProcessor::processImage(const QImage& image) const
{
  QImage *result = NULL;
  if (_gray)
  {
    switch (_type)
    {
    case Dilation:
      {
        ImageAlgorithm::GrayDilationFilter *filter =
            new ImageAlgorithm::GrayDilationFilter(_matrix.data(), _center, _width, _height);
        result = ImageAlgorithm::filtImage<ImageAlgorithm::GrayDilationFilter>(image, _area, filter);
        delete filter;
        break;
      }
    case Erosion:
      {
        ImageAlgorithm::GrayErosionFilter *filter =
            new ImageAlgorithm::GrayErosionFilter(_matrix.data(), _center, _width, _height);
        result = ImageAlgorithm::filtImage<ImageAlgorithm::GrayErosionFilter>(image, _area, filter);
        delete filter;
        break;
      }
    case Open:
      {
        ImageAlgorithm::GrayErosionFilter *filter =
            new ImageAlgorithm::GrayErosionFilter(_matrix.data(), _center, _width, _height);
        result = ImageAlgorithm::filtImage<ImageAlgorithm::GrayErosionFilter>(image, _area, filter);
        delete filter;
        break;
        ImageAlgorithm::GrayDilationFilter *filter2 =
            new ImageAlgorithm::GrayDilationFilter(_matrix.data(), _center, _width, _height);
        result = ImageAlgorithm::filtImage<ImageAlgorithm::GrayDilationFilter>(image, _area, filter2);
        delete filter2;
        break;
      }
    case Close:
      {
        ImageAlgorithm::GrayDilationFilter *filter =
            new ImageAlgorithm::GrayDilationFilter(_matrix.data(), _center, _width, _height);
        result = ImageAlgorithm::filtImage<ImageAlgorithm::GrayDilationFilter>(image, _area, filter);
        delete filter;
        break;
        ImageAlgorithm::GrayErosionFilter *filter2 =
            new ImageAlgorithm::GrayErosionFilter(_matrix.data(), _center, _width, _height);
        result = ImageAlgorithm::filtImage<ImageAlgorithm::GrayErosionFilter>(image, _area, filter2);
        delete filter2;
        break;
      }
    }
  }
  else
  {
    switch (_type)
    {
    case Dilation:
      {
        ImageAlgorithm::DilationFilter *filter =
            new ImageAlgorithm::DilationFilter(_matrix.data(), _center, _width, _height);
        result = ImageAlgorithm::filtImage<ImageAlgorithm::DilationFilter>(image, _area, filter);
        delete filter;
        break;
      }
    case Erosion:
      {
        ImageAlgorithm::ErosionFilter *filter =
            new ImageAlgorithm::ErosionFilter(_matrix.data(), _center, _width, _height);
        result = ImageAlgorithm::filtImage<ImageAlgorithm::ErosionFilter>(image, _area, filter);
        delete filter;
        break;
      }
    case Open:
      {
        ImageAlgorithm::ErosionFilter *filter =
            new ImageAlgorithm::ErosionFilter(_matrix.data(), _center, _width, _height);
        result = ImageAlgorithm::filtImage<ImageAlgorithm::ErosionFilter>(image, _area, filter);
        delete filter;
        break;
        ImageAlgorithm::DilationFilter *filter2 =
            new ImageAlgorithm::DilationFilter(_matrix.data(), _center, _width, _height);
        result = ImageAlgorithm::filtImage<ImageAlgorithm::DilationFilter>(image, _area, filter2);
        delete filter2;
        break;
      }
    case Close:
      {
        ImageAlgorithm::DilationFilter *filter =
            new ImageAlgorithm::DilationFilter(_matrix.data(), _center, _width, _height);
        result = ImageAlgorithm::filtImage<ImageAlgorithm::DilationFilter>(image, _area, filter);
        delete filter;
        break;
        ImageAlgorithm::ErosionFilter *filter2 =
            new ImageAlgorithm::ErosionFilter(_matrix.data(), _center, _width, _height);
        result = ImageAlgorithm::filtImage<ImageAlgorithm::ErosionFilter>(image, _area, filter2);
        delete filter2;
        break;
      }
    }
  }
  return result;
}

void DilationErosionProcessor::processImage(QImage *image) const
{
  if (_gray)
  {
    switch (_type)
    {
    case Dilation:
      {
        ImageAlgorithm::GrayDilationFilter *filter =
            new ImageAlgorithm::GrayDilationFilter(_matrix.data(), _center, _width, _height);
        ImageAlgorithm::filtImage<ImageAlgorithm::GrayDilationFilter>(image, _area, filter);
        delete filter;
        break;
      }
    case Erosion:
      {
        ImageAlgorithm::GrayErosionFilter *filter =
            new ImageAlgorithm::GrayErosionFilter(_matrix.data(), _center, _width, _height);
        ImageAlgorithm::filtImage<ImageAlgorithm::GrayErosionFilter>(image, _area, filter);
        delete filter;
        break;
      }
    case Open:
      {
        ImageAlgorithm::GrayErosionFilter *filter =
            new ImageAlgorithm::GrayErosionFilter(_matrix.data(), _center, _width, _height);
        ImageAlgorithm::filtImage<ImageAlgorithm::GrayErosionFilter>(image, _area, filter);
        delete filter;
        break;
        ImageAlgorithm::GrayDilationFilter *filter2 =
            new ImageAlgorithm::GrayDilationFilter(_matrix.data(), _center, _width, _height);
        ImageAlgorithm::filtImage<ImageAlgorithm::GrayDilationFilter>(image, _area, filter2);
        delete filter2;
        break;
      }
    case Close:
      {
        ImageAlgorithm::GrayDilationFilter *filter =
            new ImageAlgorithm::GrayDilationFilter(_matrix.data(), _center, _width, _height);
        ImageAlgorithm::filtImage<ImageAlgorithm::GrayDilationFilter>(image, _area, filter);
        delete filter;
        break;
        ImageAlgorithm::GrayErosionFilter *filter2 =
            new ImageAlgorithm::GrayErosionFilter(_matrix.data(), _center, _width, _height);
        ImageAlgorithm::filtImage<ImageAlgorithm::GrayErosionFilter>(image, _area, filter2);
        delete filter2;
        break;
      }
    }
  }
  else
  {
    switch (_type)
    {
    case Dilation:
      {
        ImageAlgorithm::DilationFilter *filter =
            new ImageAlgorithm::DilationFilter(_matrix.data(), _center, _width, _height);
        ImageAlgorithm::filtImage<ImageAlgorithm::DilationFilter>(image, _area, filter);
        delete filter;
        break;
      }
    case Erosion:
      {
        ImageAlgorithm::ErosionFilter *filter =
            new ImageAlgorithm::ErosionFilter(_matrix.data(), _center, _width, _height);
        ImageAlgorithm::filtImage<ImageAlgorithm::ErosionFilter>(image, _area, filter);
        delete filter;
        break;
      }
    case Open:
      {
        ImageAlgorithm::ErosionFilter *filter =
            new ImageAlgorithm::ErosionFilter(_matrix.data(), _center, _width, _height);
        ImageAlgorithm::filtImage<ImageAlgorithm::ErosionFilter>(image, _area, filter);
        delete filter;
        break;
        ImageAlgorithm::DilationFilter *filter2 =
            new ImageAlgorithm::DilationFilter(_matrix.data(), _center, _width, _height);
        ImageAlgorithm::filtImage<ImageAlgorithm::DilationFilter>(image, _area, filter2);
        delete filter2;
        break;
      }
    case Close:
      {
        ImageAlgorithm::DilationFilter *filter =
            new ImageAlgorithm::DilationFilter(_matrix.data(), _center, _width, _height);
        ImageAlgorithm::filtImage<ImageAlgorithm::DilationFilter>(image, _area, filter);
        delete filter;
        break;
        ImageAlgorithm::ErosionFilter *filter2 =
            new ImageAlgorithm::ErosionFilter(_matrix.data(), _center, _width, _height);
        ImageAlgorithm::filtImage<ImageAlgorithm::ErosionFilter>(image, _area, filter2);
        delete filter2;
        break;
      }
    }
  }
}

QDialog *DilationErosionProcessor::getOptionDialog(Area area, const MyImage& image)
{
  DilationErosionDialog *result = new DilationErosionDialog(image.getImage(),
                                                            area,
                                                            image.getType() == MyImage::Gray);
  connect(result,
          SIGNAL(confirmed(DilationErosionProcessor::DilationErosionType,
                           int,int,int,bool,Area,QVector<int>)),
          this,
          SLOT(confirm(DilationErosionProcessor::DilationErosionType,
                       int,int,int,bool,Area,QVector<int>)));
  return result;
}

QString DilationErosionProcessor::name() const
{
  return "DilationErosion";
}

QString DilationErosionProcessor::toString() const
{
  QString result = tr("%1 %2 %3 %4 %5 %6").
                   arg((int) _type).
                   arg(_width).
                   arg(_height).
                   arg(_center).
                   arg(_gray ? 1 : 0).
                   arg(_area.toString());
  for (int i = 0;i < _matrix.size();++i)
    result += tr(" %1").arg(_matrix[i]);
  return result;
}

AbstractImageProcessor *DilationErosionProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() < 6)
    return NULL;
  DilationErosionType type = (DilationErosionType)list.takeFirst().toInt();
  int width = list.takeFirst().toInt();
  int height = list.takeFirst().toInt();
  int center = list.takeFirst().toInt();
  bool gray = list.takeFirst().toInt() > 0;
  Area area = Area::fromString(list.takeFirst());
  QVector<int> matrix;
  if (list.size() != width * height)
    return NULL;
  for (int i = 0;i < width * height;++i)
    matrix.push_back(list.takeFirst().toInt());
  DilationErosionProcessor *result = new DilationErosionProcessor();
  result->_type = type;
  result->_width = width;
  result->_height = height;
  result->_center = center;
  result->_gray = gray;
  result->_matrix = matrix;
  result->_area = area;
  return result;
}

QString DilationErosionProcessor::description() const
{
  return "Dilation/Erosion/Open/Close";
}

QString DilationErosionProcessor::iconPath() const
{
  return Resource::iconDilationErosion;
}

void DilationErosionProcessor::confirm(DilationErosionProcessor::DilationErosionType type,
                                       int width,
                                       int height,
                                       int center,
                                       bool gray,
                                       Area area,
                                       QVector<int> matrix)
{
  DilationErosionProcessor *processor = new DilationErosionProcessor();
  processor->_type = type;
  processor->_width = width;
  processor->_height = height;
  processor->_center = center;
  processor->_center = center;
  processor->_gray = gray;
  processor->_matrix = matrix;
  emit processorCreated(processor);
}
