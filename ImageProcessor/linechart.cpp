#include "linechart.h"

#include <qwt_interval_symbol.h>
#include <qwt_plot.h>
#include <qwt_symbol.h>

#define PEN_WIDTH         1

LineChart::LineChart(const QString &title, const QColor &symbolColor):
    QwtPlotCurve(title)

{
  setColor(symbolColor);
}

void LineChart::setColor(const QColor &symbolColor)
{
  setRenderHint(QwtPlotCurve::RenderAntialiased);
  QColor color = symbolColor;
  QPen pen = QPen(color);
  pen.setWidthF(PEN_WIDTH);
  setPen(pen);
//  setSymbol(new QwtSymbol(QwtSymbol::Ellipse,
//          symbolColor.light(150), symbolColor, QSize(8, 8)));
}

void LineChart::setValues(const QVector<QPointF>& v)
{
  setSamples(v);
}

int LineChart::rtti() const
{
  return RttiLineChart;
}
