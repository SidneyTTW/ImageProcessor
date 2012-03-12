#ifndef XPLOTPICKER_H
#define XPLOTPICKER_H

#include <QMouseEvent>
#include <qwt_plot_picker.h>
#include <qwt_plot_canvas.h>

/**
 * A class to emit signals to show what position
 * is the mouse when the left button is pressed.
 */
class XPlotPicker : public QwtPlotPicker
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  XPlotPicker(QwtPlotCanvas *canvas);

  /**
   * Constructor.
   */
  XPlotPicker(int xAxis,
              int yAxis,
              RubberBand rubberBand,
              DisplayMode trackerMode,
              QwtPlotCanvas *canvas);

  /**
   * Set whether the picker is enabled.
   */
  inline void setEnabled(bool enabled)
  {_enabled = enabled;}

private:

  /**
   * Whether the picker is enabled.
   */
  bool _enabled;

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
   * Create a correct text to show.
   */
  virtual QwtText trackerTextF(const QPointF &pos) const;

signals:
  /**
   * Signal to show the position.
   */
  void pressAt(int x);
};

#endif // XPLOTPICKER_H
