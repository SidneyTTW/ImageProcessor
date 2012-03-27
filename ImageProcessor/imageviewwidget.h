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

  void installEventFilter(QObject *object)
  {scene->installEventFilter(object);}

  void removeEventFilter(QObject *object)
  {scene->removeEventFilter(object);}


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
  void zoomIn();
  void zoomOut();

signals:
  /**
   * Signal emitted when the image is changed.
   */
  void imageChanged(const QImage& image);
};

#endif // IMAGEVIEWWIDGET_H
