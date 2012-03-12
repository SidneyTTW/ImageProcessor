#include "xplotpicker.h"

XPlotPicker::XPlotPicker(QwtPlotCanvas *canvas) :
    QwtPlotPicker(canvas)
{
}

XPlotPicker::XPlotPicker(int xAxis,
                         int yAxis,
                         RubberBand rubberBand,
                         DisplayMode trackerMode,
                         QwtPlotCanvas *canvas) :
    QwtPlotPicker(xAxis, yAxis, rubberBand, trackerMode, canvas)
{
}

void XPlotPicker::widgetMousePressEvent(QMouseEvent *event)
{
  if (_enabled && event->button() == Qt::LeftButton)
    emit pressAt(invTransform(event->pos()).x());
}

void XPlotPicker::widgetMouseMoveEvent(QMouseEvent *event)
{
  if (_enabled && (event->buttons() & Qt::LeftButton) == Qt::LeftButton)
    emit pressAt(invTransform(event->pos()).x());
}

QwtText XPlotPicker::trackerTextF(const QPointF &pos) const
{
  return QwtText(QObject::tr("%1").arg(pos.x()));
}
