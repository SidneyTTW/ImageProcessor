#ifndef BASICSTATISTICPLOT_H
#define BASICSTATISTICPLOT_H

#include <qwt_plot.h>

#include <qwt_plot_grid.h>

class BasicStatistic;
class Histogram;
class LineChart;

/**
 * Class to draw a plot of basic statistic.
 */
class BasicStatisticPlot : public QwtPlot
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  BasicStatisticPlot(BasicStatistic statistic);

  /**
   * Destructor.
   */
  ~BasicStatisticPlot();

  /**
   * Set the statistic.
   */
  void setStatistic(BasicStatistic statistic);

private:
  /**
   * 3 line charts for RGB.
   */
  LineChart *lineCharts[3];

  /**
   * 1 histogram for gray.
   */
  Histogram *grayHistogram;

  /**
   * The grid.
   */
  QwtPlotGrid *grid;

  /**
   * Init the plot.
   */
  void init();

private slots:
  /**
   * Show or hide an item.
   */
  void showItem(QwtPlotItem *item, bool on);
};

#endif // BASICSTATISTICPLOT_H
