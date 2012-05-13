#include "rgbmapplot.h"

#include <QMouseEvent>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include "linechart.h"
#include "xyplotpicker.h"

RGBMapPlot::RGBMapPlot(QWidget *parent) : QwtPlot(parent), _mode(All)
{
  init();
}

void RGBMapPlot::init()
{
  canvas()->setFrameStyle(QFrame::Box | QFrame::Plain );
  canvas()->setLineWidth(0);
  plotLayout()->setAlignCanvasToScales(true);

  // Create the legend
  QwtLegend *l = new QwtLegend;
  l->setItemMode(QwtLegend::CheckableItem);
  insertLegend(l, QwtPlot::RightLegend);

  // Create the grid
  grid = new QwtPlotGrid();
  grid->enableX(false);
  grid->enableY(true);
  grid->enableXMin(false);
  grid->enableYMin(false);
  grid->setMajPen(QPen(QColor(100, 100, 100), 0, Qt::DotLine));
  grid->attach(this);
  setAxisTitle(QwtPlot::yLeft, tr("%1").arg("RGB"));

  QString names[] = {"Red", "Green", "Blue"};
  QColor colors[] = {QColor(255,   0,   0),
                     QColor(  0, 255,   0),
                     QColor(  0,   0, 255)};

  for (int i = 0;i < 3;++i)
  {
    lineCharts[i] = new LineChart(tr("%1").arg(names[i]), colors[i]);
    lineCharts[i]->attach(this);
    for (int j = 0;j <= MAX_COLOR_VALUE;++j)
      rgbValues[i].append(QPointF(j, j));
    lineCharts[i]->setValues(rgbValues[i]);
  }

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

  magnifier = new QwtPlotMagnifier(canvas());
  panner = new QwtPlotPanner(canvas());
  panner->setEnabled(false);
  picker = new XYPlotPicker(canvas());

  connect(picker, SIGNAL(pressAt(int, int)), SLOT(mousePressed(int, int)));
  connect(picker, SIGNAL(moveAt(int, int)), SLOT(mouseMoved(int, int)));
}

void RGBMapPlot::setControlMode(ControlMode mode)
{
  _mode = mode;
  if (mode == Pan)
  {
    panner->setEnabled(true);
    picker->setEnabled(false);
  }
  else
  {
    panner->setEnabled(false);
    picker->setEnabled(true);
  }
}

void RGBMapPlot::showItem(QwtPlotItem *item, bool on)
{
  item->setVisible(on);
  replot();
}

void RGBMapPlot::mousePressed(int x, int y)
{
  pressPos = QPoint(x, y);
}

void RGBMapPlot::mouseMoved(int x, int y)
{
  if (_mode == Pan)
    return;
  QPoint from;
  QPoint to;
  if (x > pressPos.x())
  {
    from = pressPos;
    to = QPoint(x, y);
  }
  else
  {
    from = QPoint(x, y);
    to = pressPos;
  }
  for (int i = qMax(0, from.x());i <= qMin(MAX_COLOR_VALUE, to.x());++i)
  {
    int v;

    if (to.x() - from.x() == 0)
      v = qBound(0, y, MAX_COLOR_VALUE);
    else
      v = qBound(0,
                 from.y() +
                 (i - from.x()) * (to.y() - from.y()) /
                 (to.x() - from.x()),
                 MAX_COLOR_VALUE);
    switch (_mode)
    {
    case Red:
      rgbValues[0][i] = QPointF(i, v);
      lineCharts[0]->setValues(rgbValues[0]);
      break;
    case Green:
      rgbValues[1][i] = QPointF(i, v);
      lineCharts[1]->setValues(rgbValues[1]);
      break;
    case Blue:
      rgbValues[2][i] = QPointF(i, v);
      lineCharts[2]->setValues(rgbValues[2]);
      break;
    case All:
      rgbValues[0][i] = QPointF(i, v);
      rgbValues[1][i] = QPointF(i, v);
      rgbValues[2][i] = QPointF(i, v);
      lineCharts[0]->setValues(rgbValues[0]);
      lineCharts[1]->setValues(rgbValues[1]);
      lineCharts[2]->setValues(rgbValues[2]);
      break;
    default:
      break;
    }
  }
  replot();
  emit optionChanged(rgbValues);
}

RGBMapPlot::~RGBMapPlot()
{
  for (int i = 0;i < 3;++i)
    delete lineCharts[i];
  delete grid;
  delete magnifier;
  delete panner;
  delete picker;
}
