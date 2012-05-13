#ifndef RGBMAPPLOT_H
#define RGBMAPPLOT_H

#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include "myglobal.h"

class LineChart;
class XYPlotPicker;

/**
 * Class of plot to choose the map of RGB.
 */
class RGBMapPlot : public QwtPlot
{
  Q_OBJECT
public:
  /**
   * Enum class of the mode to control the picker/zoomer.
   */
  enum ControlMode {Red, Green, Blue, All, Pan};

  /**
   * Constructor.
   */
  RGBMapPlot(QWidget *parent = NULL);

  /**
   * Destructor.
   */
  ~RGBMapPlot();

  /**
   * Set the mode of the control.
   */
  void setControlMode(ControlMode mode);

private:
  /**
   * Charts of rgb.
   */
  LineChart *lineCharts[3];

  /**
   * Values of rgb map.
   */
  QVector<QPointF> rgbValues[3];

  /**
   * The panner.
   */
  QwtPlotPanner *panner;

  /**
   * The picker.
   */
  XYPlotPicker *picker;

  /**
   * The magnifier.
   */
  QwtPlotMagnifier *magnifier;

  /**
   * The grid.
   */
  QwtPlotGrid *grid;

  /**
   * The current control mode.
   */
  ControlMode _mode;

  /**
   * Init the plot.
   */
  void init();

  /**
   * The position pressed.
   */
  QPoint pressPos;

private slots:
  /**
   * Show or hide an item.
   */
  void showItem(QwtPlotItem *item, bool on);

  /**
   * Called when the mouse is pressed and the mode isn't panner.
   */
  void mousePressed(int x, int y);

  /**
   * Called when the mouse is moved and the mode isn't panner.
   */
  void mouseMoved(int x, int y);

signals:
  /**
   * Signal emitted when the option changed.
   */
  void optionChanged(QVector<QPointF> *rgbValues);
};

#endif // RGBMAPPLOT_H
