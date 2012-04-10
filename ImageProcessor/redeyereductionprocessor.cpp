#include "redeyereductionprocessor.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QSpinBox>

RedEyeReductionProcessor::RedEyeReductionProcessor() :
    _radius(10),
    _optionWidget(NULL)
{
}

MyImage::ImageTypeFlag RedEyeReductionProcessor::resultType() const
{
  return (MyImage::ImageTypeFlag)MyImage::Remain;
}

QImage *RedEyeReductionProcessor::processImage(const QImage& image) const
{
  QImage *result = new QImage(image);
  for (int i = 0;i < positions.size();++i)
  {
    Area area(Ellipse(positions.at(i), _radius, _radius));
    ImageAlgorithm::redEyeReduction(result, area);
  }
  return result;
}

void RedEyeReductionProcessor::processImage(QImage *image) const
{
  for (int i = 0;i < positions.size();++i)
  {
    Area area(Ellipse(positions.at(i), _radius, _radius));
    ImageAlgorithm::redEyeReduction(image, area);
  }
}

QWidget *RedEyeReductionProcessor::optionWidget()
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

MyImage RedEyeReductionProcessor::preProcessImage(const MyImage& image) const
{
  QImage *resultImage = processImage(image.getImage());
  MyImage result(*resultImage, image.getType());
  delete resultImage;
  return result;
}

bool RedEyeReductionProcessor::cancelWhenNewOneIsCreated() const
{
  return false;
}

void RedEyeReductionProcessor::interrupt()
{
  positions.clear();
}

QString RedEyeReductionProcessor::name() const
{
  return "Red Eye Reduction";
}

QString RedEyeReductionProcessor::toString() const
{
  QString result = tr("%1").arg(_radius);
  for (int i = 0;i < positions.size();++i)
    result += tr(" %1 %2").arg(positions[i].x()).arg(positions[i].y());
  return result;
}

AbstractImageProcessor *RedEyeReductionProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() <= 1 || list.size() % 2 == 0)
    return NULL;
  RedEyeReductionProcessor *result = new RedEyeReductionProcessor();
  result->_radius = list.takeFirst().toInt();
  while (!list.isEmpty())
    result->positions.push_back(QPoint(list.takeFirst().toInt(),
                                       list.takeFirst().toInt()));
  return result;
}

QString RedEyeReductionProcessor::description() const
{
  return "Red Eye Reduction";
}

QString RedEyeReductionProcessor::iconPath() const
{
  return Resource::iconRedEye;
}

bool RedEyeReductionProcessor::eventFilter(QObject *object, QEvent *event)
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
      RedEyeReductionProcessor *newProcessor = new RedEyeReductionProcessor();
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

void RedEyeReductionProcessor::changeToRadius(int radius)
{
  _radius = radius;
}
