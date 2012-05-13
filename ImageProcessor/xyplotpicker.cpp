#include "xyplotpicker.h"

XYPlotPicker::XYPlotPicker(QwtPlotCanvas *canvas) :
    QwtPlotPicker(canvas)
{
}

XYPlotPicker::XYPlotPicker(int xAxis,
                         int yAxis,
                         RubberBand rubberBand,
                         DisplayMode trackerMode,
                         QwtPlotCanvas *canvas) :
    QwtPlotPicker(xAxis, yAxis, rubberBand, trackerMode, canvas)
{
}

void XYPlotPicker::widgetMousePressEvent(QMouseEvent *event)
{
  QPointF pos = invTransform(event->pos());
  emit pressAt(pos.x(), pos.y());
}

void XYPlotPicker::widgetMouseMoveEvent(QMouseEvent *event)
{
  QPointF pos = invTransform(event->pos());
  emit moveAt(pos.x(), pos.y());
}

void XYPlotPicker::widgetMouseReleaseEvent(QMouseEvent *event)
{
  QPointF pos = invTransform(event->pos());
  emit releaseAt(pos.x(), pos.y());
}

QwtText XYPlotPicker::trackerTextF(const QPointF &pos) const
{
  return QwtText(QObject::tr("%1 %2").arg(pos.x(), pos.y()));
}
