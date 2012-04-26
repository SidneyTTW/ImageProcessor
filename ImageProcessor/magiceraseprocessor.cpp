#include "magiceraseprocessor.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QSpinBox>
#include "imagealgorithm.h"
#include "myglobal.h"

MagicEraseProcessor::MagicEraseProcessor() :
    _allowedDifference(10),
    valid(false),
    _optionWidget(NULL)
{
}

MyImage::ImageTypeFlag MagicEraseProcessor::resultType() const
{
  return MyImage::Colored;
}

QImage *MagicEraseProcessor::processImage(const QImage& image) const
{
  QImage *result = new QImage(image);
  if (valid)
    ImageAlgorithm::magicErase(result, pos, _allowedDifference);
  return result;
}

void MagicEraseProcessor::processImage(QImage *image) const
{
  if (valid)
    ImageAlgorithm::magicErase(image, pos, _allowedDifference);
}

QWidget *MagicEraseProcessor::optionWidget()
{
  if (_optionWidget == NULL)
  {
    _optionWidget = new QSpinBox();
    _optionWidget->setMinimum(0);
    _optionWidget->setMaximum(255);
    _optionWidget->setValue(_allowedDifference);
    connect(_optionWidget,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(changeToAllowedDifference(int)));
  }
  return _optionWidget;
}

MyImage MagicEraseProcessor::preProcessImage(const MyImage& image) const
{
  QImage *resultImage = new QImage(image.getImage());
  if (valid)
    ImageAlgorithm::magicErase(resultImage, pos, _allowedDifference);
  MyImage result(*resultImage, MyImage::Colored);
  delete resultImage;
  return result;
}

bool MagicEraseProcessor::cancelWhenNewOneIsCreated() const
{
  return false;
}

void MagicEraseProcessor::interrupt()
{
  valid = false;
}

QString MagicEraseProcessor::name() const
{
  return "Magic Erase";
}

QString MagicEraseProcessor::toString() const
{
  return tr("%1 %2 %3").arg(_allowedDifference).arg(pos.x()).arg(pos.y());
}

AbstractImageProcessor *MagicEraseProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() != 3)
    return NULL;
  MagicEraseProcessor *result = new MagicEraseProcessor();
  result->_allowedDifference = list.takeFirst().toInt();
  result->pos.setX(list.takeFirst().toInt());
  result->pos.setY(list.takeFirst().toInt());
  result->valid = true;
  return result;
}

QString MagicEraseProcessor::description() const
{
  return "Magic erase";
}

QString MagicEraseProcessor::iconPath() const
{
  return Resource::iconMagicErase;
}

bool MagicEraseProcessor::eventFilter(QObject *object, QEvent *event)
{
  switch (event->type())
  {
  case QEvent::GraphicsSceneMousePress:
    {
      QPointF pos = ((QGraphicsSceneMouseEvent *) event)->scenePos();
      MagicEraseProcessor *newProcessor = new MagicEraseProcessor();
      newProcessor->pos = QPoint(pos.x(), pos.y());
      newProcessor->_allowedDifference = _allowedDifference;
      newProcessor->valid = true;
      emit processorCreated(newProcessor);
      break;
    }
  default:
    break;
  }
  return false;
}

void MagicEraseProcessor::changeToAllowedDifference(int allowedDifference)
{
  _allowedDifference = allowedDifference;
}
