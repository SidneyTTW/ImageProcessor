#ifndef XYPLOTPICKER_H
#define XYPLOTPICKER_H

#include <QMouseEvent>
#include <qwt_plot_picker.h>
#include <qwt_plot_canvas.h>

/**
 * A class to emit signals to show what position
 * is the mouse when the left button is pressed.
 */
class XYPlotPicker : public QwtPlotPicker
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  XYPlotPicker(QwtPlotCanvas *canvas);

  /**
   * Constructor.
   */
  XYPlotPicker(int xAxis,
               int yAxis,
               RubberBand rubberBand,
               DisplayMode trackerMode,
               QwtPlotCanvas *canvas);

protected:
  /**
   * Deal with the press event.
   */
  virtual void widgetMousePressEvent(QMouseEvent *event);

  /**
   * Deal with the move event.
   */
  virtual void widgetMouseMoveEvent(QMouseEvent *event);

  /**
   * Deal with the release event.
   */
  virtual void widgetMouseReleaseEvent(QMouseEvent *event);

  /**
   * Create a correct text to show.
   */
  virtual QwtText trackerTextF(const QPointF &pos) const;

signals:
  /**
   * Signal to show the position.
   */
  void pressAt(int x, int y);

  /**
   * Signal to show the position.
   */
  void moveAt(int x, int y);

  /**
   * Signal to show the position.
   */
  void releaseAt(int x, int y);
};

#endif // XYPLOTPICKER_H
