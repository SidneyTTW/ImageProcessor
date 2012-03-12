#ifndef LINECHART_H
#define LINECHART_H

#include <qwt_plot_curve.h>

#include <QVector>

/**
 * A class of a line chart.
 */
class LineChart : public QwtPlotCurve
{
public:
  static const int RttiLineChart = 1001;

  /**
   * Constructor.
   */
  LineChart(const QString &, const QColor &);

  /**
   * Set the color.
   */
  void setColor(const QColor &);

  /**
   * Set the values.
   *
   * @param v The values.
   */
  void setValues(const QVector<QPointF>& v);

  /**
   * The rtti to figure out which kind of chart it is.
   */
  virtual int rtti() const;
};

#endif // LINECHART_H
