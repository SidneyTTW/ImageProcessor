#include "imageviewwidget.h"

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPointF>
#include "borderlayout.h"

/**
 * Class of item to hold an image and area.
 */
class ImageItem : public QGraphicsItem
{
public:
  /**
   * Set the image of the item.
   *
   * @param image The image.
   */
  inline void setImage(const QImage& image)
  {_image = image;}

  /**
   * Set the image of the item.
   *
   * @param image The image.
   */
  inline void setArea(const Area& area)
  {_area = area;}

  virtual QRectF boundingRect() const
  {return QRectF(0, 0, _image.width(), _image.height());}

  inline bool in(const QPoint& pos)
  {return pos.x() >= 0 &&
          pos.x() < _image.width() &&
          pos.y() >= 0 &&
          pos.y() < _image.height();}

  inline QRgb pixel(const QPoint& pos)
  {return _image.pixel(pos);}

  virtual void paint(QPainter *painter,
                     const QStyleOptionGraphicsItem *,
                     QWidget *)
  {
    painter->fillRect(0,
                      0,
                      _image.width(),
                      _image.height(),
                      QColor(255, 255, 255));
    painter->drawImage(0, 0, _image);
    _area.paint(painter, _image.width(), _image.height());
  }

private:
  /**
   * The image.
   */
  QImage _image;

  /**
   * The area.
   */
  Area _area;
};

/**
 * Class of view which have different performance when dealing wheel event from
 * original QGraphicsView.
 */
class MyView : public QGraphicsView
{
public:
  /**
   * Constructor.
   */
  MyView(QGraphicsScene *scene) : QGraphicsView(scene) {}

protected:
  virtual void wheelEvent(QWheelEvent *event)
  {
    if (event->modifiers().testFlag(Qt::ControlModifier))
    {
      if (event->delta() < 0)
        scale(0.8, 0.8);
      else
        scale(1.25, 1.25);
    }
    else if (event->modifiers().testFlag(Qt::ShiftModifier))
      horizontalScrollBar()->event(event);
    else
      verticalScrollBar()->event(event);
    viewport()->update();
  }
};

ImageViewWidget::ImageViewWidget(QWidget *parent) :
    QWidget(parent)
{
  label = new QLabel();
  scene = new QGraphicsScene();
  view = new MyView(scene);
  imageItem = new ImageItem();
  scene->addItem(imageItem);
  scene->installEventFilter(this);
  imageItem->setPos(0, 0);
  BorderLayout *layout = new BorderLayout();
  layout->addWidget(label, BorderLayout::North);
  layout->addWidget(view, BorderLayout::Center);
  setLayout(layout);

  view->setMouseTracking(true);
}

void ImageViewWidget::setImage(const QImage &image)
{
  imageItem->setImage(image);
  view->viewport()->update();
  emit imageChanged(image);
}

void ImageViewWidget::setArea(const Area& area)
{
  imageItem->setArea(area);
  update();
}

void ImageViewWidget::zoomIn()
{
  view->scale(1.25, 1.25);
}

void ImageViewWidget::zoomOut()
{
  view->scale(0.8, 0.8);
}

bool ImageViewWidget::eventFilter(QObject *object, QEvent *event)
{
  if (object != scene)
    return false;
  if (event->type() == QEvent::GraphicsSceneMouseMove)
  {
    QPointF posF = ((QGraphicsSceneMouseEvent *) event)->scenePos();
    QPoint pos = QPoint(posF.x(), posF.y());
    if (!imageItem->in(pos))
      return false;
    QRgb rgb = imageItem->pixel(pos);
    label->setText(tr("x: %1 y:%2 r:%3 g:%4 b:%5 a:%6").
                   arg(pos.x()).
                   arg(pos.y()).
                   arg(qRed(rgb)).
                   arg(qGreen(rgb)).
                   arg(qBlue(rgb)).
                   arg(qAlpha(rgb)));
  }
  return false;
}

ImageViewWidget::~ImageViewWidget()
{
  delete label;
  delete imageItem;
  delete scene;
  delete view;
  delete layout();
}
