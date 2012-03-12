#include "histogram.h"

#include <QBrush>
#include <QPen>

Histogram::Histogram(const QString &title, const QColor &color):
    QwtPlotHistogram(title)
{
  setStyle(QwtPlotHistogram::Columns);
  setColor(color);
}

void Histogram::setColor(const QColor &color)
{
  setPen(QPen(color));
  setBrush(QBrush(color));
}

void Histogram::setValue(const double x, const double y)
{
  QVector<QwtIntervalSample> samples(1);
  QwtInterval interval(x - 0.4, x + 0.4);
  interval.setBorderFlags(QwtInterval::ExcludeMaximum);
  samples[0] = QwtIntervalSample(y, interval);
  setData(new QwtIntervalSeriesData(samples));
}
