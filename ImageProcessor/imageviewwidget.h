#ifndef IMAGEVIEWWIDGET_H
#define IMAGEVIEWWIDGET_H

#include <QWidget>

#include <QGraphicsView>
#include <QImage>
#include <QLabel>
#include <QScrollBar>
#include <QWheelEvent>
#include "area.h"

class ImageItem;
class MyView;

/**
 * Class of widget to show an image.
 */
class ImageViewWidget : public QWidget
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  explicit ImageViewWidget(QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~ImageViewWidget();

  /**
   * Set the image to show.
   */
  void setImage(const QImage& image);

  /**
   * Set the selected area.
   */
  void setArea(const Area& area);

  /**
   * @return The selected area.
   */
  Area getArea() const;

  /**
   * Insert the event filter to the scene instead of "this".
   *
   * @warning It's not a virtual void.
   */
  void installEventFilter(QObject *object)
  {scene->installEventFilter(object);}

  /**
   * Remove the event filter from the scene instead of "this".
   *
   * @warning It's not a virtual void.
   */
  void removeEventFilter(QObject *object)
  {scene->removeEventFilter(object);}

  /**
   * @return The graphics scene to hold the image and the selected area.
   */
  const QGraphicsScene *getScene() const
  {return scene;}

protected:
  virtual bool eventFilter(QObject *object, QEvent *event);

private:
  /**
   * The image item.
   */
  ImageItem *imageItem;

  /**
   * The label to show the position and the RGBA.
   */
  QLabel *label;

  /**
   * The graphics scene to hold the image and the selected area.
   */
  QGraphicsScene *scene;

  /**
   * The graphics view to hold the scene.
   */
  MyView *view;

public slots:
  /**
   * Zoom in.
   */
  void zoomIn();

  /**
   * Zoom out.
   */
  void zoomOut();

signals:
  /**
   * Signal emitted when the image is changed.
   */
  void imageChanged(const QImage& image);

  /**
   * Signal emitted when the area is changed.
   */
  void areaChanged(const Area& area);
};

#endif // IMAGEVIEWWIDGET_H
