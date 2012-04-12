#include "algebraoperationprocessor.h"

#include "algebraoperationdialog.h"

AlgebraOperationProcessor::AlgebraOperationProcessor()
{
}

Area::AreaType AlgebraOperationProcessor::acceptableAreaType() const
{
  return Area::TypeEmpty;
}

Area::AreaTypeFlag AlgebraOperationProcessor::resultAreaType() const
{
  return Area::TypeEmpty;
}

Area AlgebraOperationProcessor::resultArea() const
{
  return Area();
}

MyImage::ImageType AlgebraOperationProcessor::acceptableType() const
{
  return (MyImage::ImageType)(MyImage::Colored |
                              MyImage::Gray |
                              MyImage::BlackAndWhite);
}

MyImage::ImageTypeFlag AlgebraOperationProcessor::resultType() const
{
  return MyImage::Colored;
}

QImage *AlgebraOperationProcessor::processImage(const QImage& image) const
{
  QImage another = QImage(_path).convertToFormat(QImage::Format_ARGB32);
  QImage *result = ImageAlgorithm::algebraOperation(image, another, _type);
  return result;
}

void AlgebraOperationProcessor::processImage(QImage *image) const
{
  QImage another = QImage(_path).convertToFormat(QImage::Format_ARGB32);
  ImageAlgorithm::algebraOperation(image, another, _type);
}

QDialog *AlgebraOperationProcessor::getOptionDialog(Area area, const MyImage& image)
{
  AlgebraOperationDialog *result = new AlgebraOperationDialog(image.getImage());
  connect(result,
          SIGNAL(confirm(ImageAlgorithm::AlgebraOperationType,QString)),
          this,
          SLOT(confirm(ImageAlgorithm::AlgebraOperationType,QString)));
  return result;
}

QString AlgebraOperationProcessor::name() const
{
  return "Algebra Operation";
}

QString AlgebraOperationProcessor::toString() const
{
  QString path = _path;
  path.replace(QChar(' '), QChar('*'));
  QString result = tr("%1 %2").arg((int) _type).arg(path);
  return result;
}

AbstractImageProcessor *AlgebraOperationProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() != 2)
    return NULL;
  AlgebraOperationProcessor *result = new AlgebraOperationProcessor();
  result->_type = (ImageAlgorithm::AlgebraOperationType) list.takeFirst().toInt();
  result->_path = list.takeFirst().replace(QChar('*'), QChar(' '));;
  return result;
}

QString AlgebraOperationProcessor::description() const
{
  return "Do algebra operation on two images";
}

QString AlgebraOperationProcessor::iconPath() const
{
  return Resource::iconAlgebraOperation;
}

void AlgebraOperationProcessor::confirm
    (ImageAlgorithm::AlgebraOperationType type,
     QString path)
{
  AlgebraOperationProcessor *processor = new AlgebraOperationProcessor();
  processor->_type = type;
  processor->_path = path;
  emit processorCreated(processor);
}

