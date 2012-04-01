#ifndef AREACHOOSER_H
#define AREACHOOSER_H

#include <QObject>

#include <QPoint>
#include <QVector>

class ImageViewWidget;

/**
 * Class to choose area in the image view widget.
 * Can only deal with one image view widget at a time.
 */
class AreaChooser : public QObject
{
  Q_OBJECT
public:
  /**
   * Type to choose the area.
   */
  enum AreaChooseType {TypeRectangle, TypePolygon, TypeEllipse};

  /**
   * Constructor.
   */
  explicit AreaChooser(QObject *parent = 0);

  /**
   * Attach a widget.
   */
  void attach(ImageViewWidget *widget);

  /**
   * Detach the widget.
   */
  void detach();

  /**
   * Set type of the area chooser.
   *
   * @param type The type of area to choose.
   */
  void setType(AreaChooseType type);

protected:
  virtual bool eventFilter(QObject *object, QEvent *event);

private:
  /**
   * Type to choose area.
   */
  AreaChooseType _type;

  /**
   * Key points of the area.
   */
  QVector<QPoint> keyPoints;

  /**
   * The widget attached.
   */
  ImageViewWidget *_widget;

signals:

public slots:

};

#endif // AREACHOOSER_H
