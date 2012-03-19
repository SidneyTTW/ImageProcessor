#include "convolutionprocessor.h"

#include "imagealgorithm.h"
#include "convolutiondialog.h"

ConvolutionProcessor::ConvolutionProcessor()
{
}

Area::AreaType ConvolutionProcessor::acceptableAreaType() const
{
  return Area::TypeEmpty;
}

Area::AreaTypeFlag ConvolutionProcessor::resultAreaType() const
{
  return Area::TypeEmpty;
}

Area ConvolutionProcessor::resultArea() const
{
  return Area();
}

MyImage::ImageType ConvolutionProcessor::acceptableType() const
{
  return (MyImage::ImageType)MyImage::Gray;
}

MyImage::ImageTypeFlag ConvolutionProcessor::resultType() const
{
  return MyImage::Remain;
}

QImage *ConvolutionProcessor::processImage(const QImage& image) const
{
  QImage *result = ImageAlgorithm::convolution(image,
                                               matrix,
                                               matrixWidth,
                                               divisor,
                                               offset);
  return result;
}

void ConvolutionProcessor::processImage(QImage *image) const
{
  ImageAlgorithm::convolution(image,
                              matrix,
                              matrixWidth,
                              divisor,
                              offset);
}

QDialog *ConvolutionProcessor::getOptionDialog(Area area,
                                               const MyImage& image)
{
  ConvolutionDialog *result = new ConvolutionDialog(image);
  connect(result,
          SIGNAL(confirm(QVector<int>,int,int,int)),
          this,
          SLOT(confirm(QVector<int>,int,int,int)));
  return result;
}

QString ConvolutionProcessor::name() const
{
  return "Convolution";
}

QString ConvolutionProcessor::toString() const
{
  QString result = tr("%1 %2 %3").arg(matrixWidth).arg(divisor).arg(offset);
  for (int i = 0;i < matrix.size();++i)
    result += tr(" %1").arg(matrix[i]);
  return result;
}

AbstractImageProcessor *ConvolutionProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() < 4)
    return NULL;
  ConvolutionProcessor *result = new ConvolutionProcessor();
  result->matrixWidth =  list.takeFirst().toInt();
  result->divisor = list.takeFirst().toInt();
  result->offset = list.takeFirst().toInt();
  while (!list.isEmpty())
      result->matrix.push_back(list.takeFirst().toInt());
  return result;
}

void ConvolutionProcessor::confirm(QVector<int> matrix,
                                   int matrixWidth,
                                   int divisor,
                                   int offset)
{
  ConvolutionProcessor *processor = new ConvolutionProcessor();
  processor->matrix = matrix;
  processor->matrixWidth = matrixWidth;
  processor->divisor = divisor;
  processor->offset = offset;
  emit processorCreated(processor);
}
