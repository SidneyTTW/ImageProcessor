#include "eraseprocessor.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QSpinBox>
#include "imageviewwidget.h"

EraseProcessor::EraseProcessor() :
    _size(10),
    _optionWidget(NULL)
{
}

MyImage::ImageTypeFlag EraseProcessor::resultType() const
{
  return (MyImage::ImageTypeFlag)MyImage::Remain;
}

QImage *EraseProcessor::processImage(const QImage& image) const
{
  QColor color;
  if (getCurrentColor().isValid())
    color = getBackgroundColor();
  else
    color = _color;
  QImage *result = new QImage(image);
  for (int i = 0;i < positions.size();++i)
  {
    Area area(QRect(positions.at(i).x() - _size / 2,
                    positions.at(i).y() - _size / 2,
                    _size + 1,
                    _size + 1));
    ImageAlgorithm::erase(result, area, color);
  }
  return result;
}

void EraseProcessor::processImage(QImage *image) const
{
  QColor color;
  if (getCurrentColor().isValid())
    color = getBackgroundColor();
  else
    color = _color;
  for (int i = 0;i < positions.size();++i)
  {
    Area area(QRect(positions.at(i).x() - _size / 2,
                    positions.at(i).y() - _size / 2,
                    _size + 1,
                    _size + 1));
    ImageAlgorithm::erase(image, area, color);
  }
}

QWidget *EraseProcessor::optionWidget()
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
            SLOT(changeToSize(int)));
  }
  return _optionWidget;
}

MyImage EraseProcessor::preProcessImage(const MyImage& image) const
{
  QImage *resultImage = processImage(image.getImage());
  MyImage result(*resultImage, image.getType());
  delete resultImage;
  return result;
}

bool EraseProcessor::cancelWhenNewOneIsCreated() const
{
  return false;
}

void EraseProcessor::interrupt()
{
  positions.clear();
}

QString EraseProcessor::name() const
{
  return "Erase";
}

QString EraseProcessor::toString() const
{
  QString result = tr("%1 %2").arg(_color.rgba()).arg(_size);
  for (int i = 0;i < positions.size();++i)
    result += tr(" %1 %2").arg(positions[i].x()).arg(positions[i].y());
  return result;
}

AbstractImageProcessor *EraseProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() <= 2 || list.size() % 2 == 1)
    return NULL;
  EraseProcessor *result = new EraseProcessor();
  result->_color = QColor(list.takeFirst().toUInt());
  result->_size = list.takeFirst().toInt();
  while (!list.isEmpty())
    result->positions.push_back(QPoint(list.takeFirst().toInt(),
                                       list.takeFirst().toInt()));
  return result;
}

QString EraseProcessor::description() const
{
  return "Erase";
}

QString EraseProcessor::iconPath() const
{
  return Resource::iconErase;
}

bool EraseProcessor::eventFilter(QObject *object, QEvent *event)
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
      scene->setCursorArea(QRect(pos.x() - _size / 2,
                                 pos.y() - _size / 2,
                                 _size,
                                 _size));
      if (((QGraphicsSceneMouseEvent *) event)->buttons() == Qt::NoButton)
        break;
      positions.push_back(QPoint(pos.x(), pos.y()));
      emit optionChanged(this);
      break;
    }
  case QEvent::GraphicsSceneMouseRelease:
    {
      EraseProcessor *newProcessor = new EraseProcessor();
      newProcessor->_size = _size;
      newProcessor->_colorChooser = _colorChooser;
      newProcessor->positions = positions;
      emit processorCreated(newProcessor);
    }
    break;
  default:
    break;
  }
  return false;
}

void EraseProcessor::changeToSize(int size)
{
  _size = size;
}
