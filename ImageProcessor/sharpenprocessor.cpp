#include "sharpenprocessor.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QSpinBox>
#include "imageviewwidget.h"

int SharpenProcessor::_convolutionCore[9] = { 0, -1,  0,
                                             -1,  5, -1,
                                              0, -1,  0};

SharpenProcessor::SharpenProcessor() :
    _radius(10),
    _optionWidget(NULL)
{
}

MyImage::ImageTypeFlag SharpenProcessor::resultType() const
{
  return (MyImage::ImageTypeFlag)MyImage::Remain;
}

QImage *SharpenProcessor::processImage(const QImage& image) const
{
  int divisor = 1;
  int offset = 0;
  QImage *result = new QImage(image);
  for (int i = 0;i < positions.size();++i)
  {
    ImageAlgorithm::ConvolutionFilter *filter =
        new ImageAlgorithm::ConvolutionFilter(_convolutionCore,
                                              3,
                                              3,
                                              divisor,
                                              offset);
    Area area(Ellipse(positions.at(i), _radius, _radius));
    ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(result, area, filter);
    delete filter;
  }
  return result;
}

void SharpenProcessor::processImage(QImage *image) const
{
  int divisor = 1;
  int offset = 0;
  for (int i = 0;i < positions.size();++i)
  {
    ImageAlgorithm::ConvolutionFilter *filter =
        new ImageAlgorithm::ConvolutionFilter(_convolutionCore,
                                              3,
                                              3,
                                              divisor,
                                              offset);
    Area area(Ellipse(positions.at(i), _radius, _radius));
    ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(image, area, filter);
    delete filter;
  }
}

QWidget *SharpenProcessor::optionWidget()
{
  if (_optionWidget == NULL)
  {
    _optionWidget = new QSpinBox();
    _optionWidget->setMinimum(1);
    _optionWidget->setValue(10);;
    _optionWidget->setMaximum(2000);
    connect(_optionWidget,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(changeToRadius(int)));
  }
  return _optionWidget;
}

MyImage SharpenProcessor::preProcessImage(const MyImage& image) const
{
  QImage *resultImage = processImage(image.getImage());
  MyImage result(*resultImage, image.getType());
  delete resultImage;
  return result;
}

bool SharpenProcessor::cancelWhenNewOneIsCreated() const
{
  return false;
}

void SharpenProcessor::interrupt()
{
  positions.clear();
}

QString SharpenProcessor::name() const
{
  return "Sharpen";
}

QString SharpenProcessor::toString() const
{
  QString result = tr("%1").arg(_radius);
  for (int i = 0;i < positions.size();++i)
    result += tr(" %1 %2").arg(positions[i].x()).arg(positions[i].y());
  return result;
}

AbstractImageProcessor *SharpenProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() <= 1 || list.size() % 2 == 0)
    return NULL;
  SharpenProcessor *result = new SharpenProcessor();
  result->_radius = list.takeFirst().toInt();
  while (!list.isEmpty())
    result->positions.push_back(QPoint(list.takeFirst().toInt(),
                                       list.takeFirst().toInt()));
  return result;
}

QString SharpenProcessor::description() const
{
  return "Sharpen";
}

QString SharpenProcessor::iconPath() const
{
  return Resource::iconSharpen;
}

bool SharpenProcessor::eventFilter(QObject *object, QEvent *event)
{
  MyScene *scene = (MyScene *)object;
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
      scene->setCursorArea(Ellipse(QPoint(pos.x(), pos.y()), _radius, _radius));
      if (((QGraphicsSceneMouseEvent *) event)->buttons() == Qt::NoButton)
        break;
      positions.push_back(QPoint(pos.x(), pos.y()));
      emit optionChanged(this);
      break;
    }
  case QEvent::GraphicsSceneMouseRelease:
    {
      SharpenProcessor *newProcessor = new SharpenProcessor();
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

void SharpenProcessor::changeToRadius(int radius)
{
  _radius = radius;
}
