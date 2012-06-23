#ifndef IMAGEVIEWWIDGET_H
#define IMAGEVIEWWIDGET_H

#include <QWidget>

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QImage>
#include <QLabel>
#include <QScrollBar>
#include <QWheelEvent>
#include "area.h"

class ImageItem;
class MyScene;
class MyView;

/**
 * Class of scene which can show the area which cursor effects.
 */
class MyScene : public QGraphicsScene
{
public:
  /**
   * Constructor.
   */
  MyScene(QGraphicsItem *imageItem)
  {
    addItem(imageItem);
    cursorItem = new CursorItem();
    addItem(cursorItem);
  }

  /**
   * Destructor.
   */
  ~MyScene()
  {
    removeItem(cursorItem);
    delete cursorItem;
  }

  /**
   * Set the cursor area to ellipse.
   *
   * @param ellipse The ellipse
   */
  void setCursorArea(Ellipse ellipse);

  /**
   * Set the cursor area to rectangle.
   *
   * @param rectangle The rectangle
   */
  void setCursorArea(QRect rectangle);

  /**
   * Clear the cursor area.
   */
  void clearCursorArea();

private:
  /**
   * Class of item to draw the area which cursor effects.
   */
  class CursorItem : public QGraphicsItem
  {
  public:
    /**
     * The state of the area.
     */
    enum AreaState {Non, StateEllipse, StateRectangle};

    /**
     * Constructor.
     */
    CursorItem() : areaState(Non) {}

    /**
     * Set the cursor area to ellipse.
     *
     * @param ellipse The ellipse
     */
    inline void setCursorArea(Ellipse ellipse)
    {
      _ellipse = ellipse;
      areaState = StateEllipse;
    }

    /**
   * Set the cursor area to rectangle.
   *
   * @param rectangle The rectangle
     */
    inline void setCursorArea(QRect rectangle)
    {
      _rectangle = rectangle;
      areaState = StateRectangle;
    }

    /**
     * Clear the cursor area.
     */
    inline void clearCursorArea()
    {areaState = Non;}

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *,
                       QWidget *);

    virtual QRectF boundingRect() const;

  private:
    /**
     * The ellipse.
     */
    Ellipse _ellipse;

    /**
     * The ellipse.
     */
    QRectF _rectangle;

    /**
     * The state of the area.
     */
    AreaState areaState;
  };

  /**
   * The item used to show the area.
   */
  CursorItem *cursorItem;
};

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
   * Clear the cursor area.
   */
  void clearCursorArea()
  {scene->clearCursorArea();}

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
  MyScene *scene;

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
