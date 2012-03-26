#include "basicstatisticplot.h"

#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include "basicstatistic.h"
#include "histogram.h"
#include "linechart.h"

BasicStatisticPlot::BasicStatisticPlot(BasicStatistic statistic)
{
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
  init();
  setStatistic(statistic);
}

void BasicStatisticPlot::init()
{
  canvas()->setFrameStyle(QFrame::Box | QFrame::Plain );
  canvas()->setLineWidth(0);
  plotLayout()->setAlignCanvasToScales(true);

  // Create the legend
  QwtLegend *l = new QwtLegend;
  l->setItemMode(QwtLegend::CheckableItem);
  insertLegend(l, QwtPlot::RightLegend);

  // Create the grid
  QwtPlotGrid *grid = new QwtPlotGrid();
  grid->enableX(false);
  grid->enableY(true);
  grid->enableXMin(false);
  grid->enableYMin(false);
  grid->setMajPen(QPen(QColor(100, 100, 100), 0, Qt::DotLine));
  grid->attach(this);
  enableAxis(QwtPlot::yRight, true);
  setAxisTitle(QwtPlot::yLeft, tr("%1").arg("RGB"));
  setAxisTitle(QwtPlot::yRight, tr("%1").arg("Gray"));

  QString names[] = {"Red", "Green", "Blue", "Gray"};
  QColor colors[] = {QColor(255,   0,   0),
                     QColor(  0, 255,   0),
                     QColor(  0,   0, 255),
                     QColor(100, 100, 100)};

  for (int i = 0;i < 3;++i)
  {
    lineCharts[i] = new LineChart(tr("%1").arg(names[i]), colors[i]);
    lineCharts[i]->attach(this);
  }
  grayHistogram = new Histogram(tr("%1").arg(names[3]), colors[3]);
  grayHistogram->setAxes(QwtPlot::xBottom, QwtPlot::yRight);
  grayHistogram->attach(this);

  QwtPlotItemList items = itemList(QwtPlotItem::Rtti_PlotItem);
  for (int j = 0;j < items.size();++j)
  {
    QwtLegendItem *legendItem = (QwtLegendItem *)legend()->find(items[j]);
    if (legendItem)
      legendItem->setChecked(true);
    items[j]->setVisible(true);
  }

  connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)),
      SLOT(showItem(QwtPlotItem *, bool)));
}

void BasicStatisticPlot::setStatistic(BasicStatistic statistic)
{
  int maxY = 0;
  for (int i = 0;i < 3;++i)
  {
    QVector<QPointF> values;
    for (int j = 0;j <= MAX_COLOR_VALUE;++j)
    {
      values.append(QPointF(j, statistic.counts[i][j]));
      if (statistic.counts[i][j] > maxY)
        maxY = statistic.counts[i][j];
    }
    lineCharts[i]->setValues(values);
  }
  QVector<QwtIntervalSample> samples;
  for (int i = 0;i <= MAX_COLOR_VALUE;++i)
  {
    QwtInterval interval(i - 0.4, i + 0.4);
    interval.setBorderFlags(QwtInterval::ExcludeMaximum);
    samples.push_back(QwtIntervalSample(statistic.counts[3][i], interval));
    if (statistic.counts[3][i] > maxY)
      maxY = statistic.counts[3][i];
  }
  grayHistogram->setSamples(samples);
  setAxisScale(QwtPlot::yLeft, 0, maxY * 1.1);
  setAxisScale(QwtPlot::xBottom, 0, MAX_COLOR_VALUE);
  replot();
}

void BasicStatisticPlot::showItem(QwtPlotItem *item, bool on)
{
  item->setVisible(on);
  replot();
}

BasicStatisticPlot::~BasicStatisticPlot()
{
  for (int i = 0;i < 3;++i)
    delete lineCharts[i];
  delete grayHistogram;
}
