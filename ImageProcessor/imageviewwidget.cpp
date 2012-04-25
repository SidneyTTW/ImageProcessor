#include "imageviewwidget.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPointF>
#include "borderlayout.h"

void MyScene::setCursorArea(Ellipse ellipse)
{
  cursorItem->setCursorArea(ellipse);
  QList <QGraphicsView *> _views = views();
  QGraphicsView *view;
  foreach (view, _views)
    view->viewport()->update();
}

void MyScene::setCursorArea(QRect rectangle)
{
  cursorItem->setCursorArea(rectangle);
  QList <QGraphicsView *> _views = views();
  QGraphicsView *view;
  foreach (view, _views)
    view->viewport()->update();
}

void MyScene::clearCursorArea()
{
  cursorItem->clearCursorArea();
  QList <QGraphicsView *> _views = views();
  QGraphicsView *view;
  foreach (view, _views)
    view->viewport()->update();
}

void MyScene::CursorItem::paint(QPainter *painter,
                                const QStyleOptionGraphicsItem *,
                                QWidget *)
{
  QPen pen(QColor(0, 0, 0));
  pen.setStyle(Qt::SolidLine);
  painter->setPen(pen);
  switch (areaState)
  {
  case StateEllipse:
    {
      QPointF center = _ellipse.getCenter();
      center.setX(center.x() + 0.5);
      center.setY(center.y() + 0.5);
      painter->drawEllipse(center,
                           _ellipse.getDx() + 1,
                           _ellipse.getDy() + 1);
    }
    break;
  case StateRectangle:
    painter->drawRect(_rectangle);
    break;
  default:
    break;
  }
}


QRectF MyScene::CursorItem::boundingRect() const
{
  switch (areaState)
  {
  case StateEllipse:
    return QRectF(_ellipse.getCenter().x() - _ellipse.getDx(),
                  _ellipse.getCenter().y() - _ellipse.getDy(),
                  _ellipse.getDx() * 2,
                  _ellipse.getDy() * 2);
    break;
  case StateRectangle:
    return QRectF(_rectangle);
    break;
  default:
    return QRectF();
    break;
  }
}

/**
 * Class of item to hold an image and area.
 */
class ImageItem : public QGraphicsItem
{
public:
  /**
   * Constructor.
   */
  ImageItem() : QGraphicsItem()
  {
    bgBrush = QBrush(QPixmap(":/images/bg_pattern.png"));
  }

  /**
   * Set the image of the item.
   *
   * @param image The image.
   */
  inline void setImage(const QImage& image)
  {_image = image;}

  /**
   * Set the area of the item.
   *
   * @param area The area.
   */
  inline void setArea(const Area& area)
  {_area = area;}

  /**
   * @return The area of the item.
   */
  inline Area getArea() const
  {return _area;}

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
    painter->fillRect(0, 0, _image.width(), _image.height(), bgBrush);
    painter->drawImage(0, 0, _image);
    QPen pen(QColor(150, 150, 150));
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
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

  /**
   * Brush of the background.
   */
  QBrush bgBrush;
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
  MyView(QGraphicsScene *scene) : QGraphicsView(scene)
  {
    setBackgroundBrush(QBrush(QColor(100, 100, 100)));
  }

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
  imageItem = new ImageItem();
  scene = new MyScene(imageItem);
  scene->installEventFilter(this);
  view = new MyView(scene);
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
  view->setSceneRect(0, 0, image.width(), image.height());
  view->viewport()->update();
  emit imageChanged(image);
}

void ImageViewWidget::setArea(const Area& area)
{
  imageItem->setArea(area);
  view->viewport()->update();
  emit areaChanged(area);
}

Area ImageViewWidget::getArea() const
{
  return imageItem->getArea();
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
