#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <qwt_plot_histogram.h>

#include <QString>
#include <QColor>

/**
 * A class of a histogram.
 */
class Histogram: public QwtPlotHistogram
{
public:
  /**
   * Constructor.
   */
  Histogram(const QString &title, const QColor &color);

  /**
   * Set the color.
   */
  void setColor(const QColor &color);

  /**
   * Set the value.
   */
  void setValue(const double x, const double y);
};

#endif // HISTOGRAM_H
