#include "fastgaussprocessor.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QSpinBox>

FastGaussProcessor::FastGaussProcessor() :
    _radius(10),
    _optionWidget(NULL)
{
}

MyImage::ImageTypeFlag FastGaussProcessor::resultType() const
{
  return (MyImage::ImageTypeFlag)MyImage::Remain;
}

QImage *FastGaussProcessor::processImage(const QImage& image) const
{
  int width = 3;
  int *gaussMatrix = new int[width * width];
  ImageAlgorithm::gaussCore(1, 1, gaussMatrix);
  int divisor = 0;
  for (int i = 0;i < width * width;++i)
    divisor += gaussMatrix[i];
  int offset = 0;
  QImage *result = new QImage(image);
  for (int i = 0;i < positions.size();++i)
  {
    ImageAlgorithm::ConvolutionFilter *filter =
        new ImageAlgorithm::ConvolutionFilter(gaussMatrix,
                                              width,
                                              width,
                                              divisor,
                                              offset);
    Area area(Ellipse(positions.at(i), _radius, _radius));
    ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(result, area, filter);
    delete filter;
  }
  delete gaussMatrix;
  return result;
}

void FastGaussProcessor::processImage(QImage *image) const
{
  int width = 3;
  int *gaussMatrix = new int[width * width];
  ImageAlgorithm::gaussCore(1, 1, gaussMatrix);
  int divisor = 0;
  for (int i = 0;i < width * width;++i)
    divisor += gaussMatrix[i];
  int offset = 0;
  for (int i = 0;i < positions.size();++i)
  {
    ImageAlgorithm::ConvolutionFilter *filter =
        new ImageAlgorithm::ConvolutionFilter(gaussMatrix,
                                              width,
                                              width,
                                              divisor,
                                              offset);
    Area area(Ellipse(positions.at(i), _radius, _radius));
    ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(image, area, filter);
    delete filter;
  }
  delete gaussMatrix;
}

QWidget *FastGaussProcessor::optionWidget()
{
  if (_optionWidget == NULL)
  {
    _optionWidget = new QSpinBox();
    _optionWidget->setMinimum(1);
    _optionWidget->setValue(10);;
    _optionWidget->setMaximum(200);
    connect(_optionWidget,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(changeToRadius(int)));
  }
  return _optionWidget;
}

MyImage FastGaussProcessor::preProcessImage(const MyImage& image) const
{
  QImage *resultImage = processImage(image.getImage());
  MyImage result(*resultImage, image.getType());
  delete resultImage;
  return result;
}

bool FastGaussProcessor::cancelWhenNewOneIsCreated() const
{
  return false;
}

void FastGaussProcessor::interrupt()
{
}

QString FastGaussProcessor::name() const
{
  return "Fast Gauss";
}

QString FastGaussProcessor::toString() const
{
  QString result = tr("%1").arg(_radius);
  for (int i = 0;i < positions.size();++i)
    result += tr(" %1 %2").arg(positions[i].x()).arg(positions[i].y());
  return result;
}

AbstractImageProcessor *FastGaussProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() <= 1 || list.size() % 2 == 0)
    return NULL;
  FastGaussProcessor *result = new FastGaussProcessor();
  result->_radius = list.takeFirst().toInt();
  while (!list.isEmpty())
    result->positions.push_back(QPoint(list.takeFirst().toInt(),
                                       list.takeFirst().toInt()));
  return result;
}

QString FastGaussProcessor::description() const
{
  return "Blur";
}

QString FastGaussProcessor::iconPath() const
{
  return Resource::iconBlur;
}

bool FastGaussProcessor::eventFilter(QObject *object, QEvent *event)
{
  switch (event->type())
  {
  case QEvent::GraphicsSceneMousePress:
    {
      QPointF pos = ((QGraphicsSceneMouseEvent *) event)->scenePos();
      positions.clear();
      positions.push_back(QPoint(pos.x(), pos.y()));
      emit optionChanged(this);
      break;
    }
  case QEvent::GraphicsSceneMouseMove:
    {
      QPointF pos = ((QGraphicsSceneMouseEvent *) event)->scenePos();
      if (((QGraphicsSceneMouseEvent *) event)->buttons() == Qt::NoButton)
        break;
      positions.push_back(QPoint(pos.x(), pos.y()));
      emit optionChanged(this);
      break;
    }
  case QEvent::GraphicsSceneMouseRelease:
    {
      FastGaussProcessor *newProcessor = new FastGaussProcessor();
      newProcessor->_radius = _radius;
      newProcessor->positions = positions;
      emit processorCreated(newProcessor);
    }
    break;
  default:
    break;
  }
  return false;
}

void FastGaussProcessor::changeToRadius(int radius)
{
  _radius = radius;
}
