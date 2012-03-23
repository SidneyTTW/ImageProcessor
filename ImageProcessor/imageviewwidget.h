#ifndef IMAGEVIEWWIDGET_H
#define IMAGEVIEWWIDGET_H

#include <QWidget>

#include <QImage>
#include <QLabel>
#include <QScrollBar>

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
   * Convert the given position to the position in the image.
   *
   * @param point The given position.
   * @return The position in the image.
   */
  QPoint toImagePosition(QPoint point);

protected:
  virtual void wheelEvent(QWheelEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void paintEvent(QPaintEvent *event);
  virtual void resizeEvent(QResizeEvent *event);

private:
  /**
   * The image.
   */
  QImage _image;

  /**
   * The horizon bar.
   */
  QScrollBar *horizonBar;

  /**
   * The vertical bar.
   */
  QScrollBar *verticalBar;

  /**
   * The scale in X direction.
   */
  double scaleX;

  /**
   * The scale in Y direction.
   */
  double scaleY;

  /**
   * Original width.
   */
  int ow;

  /**
   * Original height.
   */
  int oh;

  /**
   * Current width.
   */
  int cw;

  /**
   * Current height.
   */
  int ch;

  /**
   * The label to show the position and the RGBA.
   */
  QLabel *label;

  /**
   * Synchronous width and height.
   */
  void synchronous();

  /**
   * Current height.
   */
  void resetScrollBars();

public slots:
  void hBarChangedSlot(int value);
  void vBarChangedSlot(int value);
  void zoomIn();
  void zoomOut();
  void resume();
  void clear();

signals:
  /**
   * Signal emitted when the image is changed.
   */
  void imageChanged(const QImage& image);
};

#endif // IMAGEVIEWWIDGET_H
