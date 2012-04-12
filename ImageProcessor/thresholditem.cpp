#include "thresholditem.h"

#include <QPainter>
#include <QMouseEvent>
#include "qwt_plot_canvas.h"
#include "qwt_scale_div.h"
#include "myglobal.h"

ThresholdItem::ThresholdItem() :
    _startColor(0),
    draggingValue(-1)
{
}

int ThresholdItem::rtti() const
{
  return QwtPlotItem::Rtti_PlotUserItem;
}

void ThresholdItem::draw(QPainter *painter,
                         const QwtScaleMap &xMap,
                         const QwtScaleMap &yMap,
                         const QRectF &rect) const
{
  int lower = rect.top();
  int upper = rect.bottom();
  int currentColor = _startColor;
  if (currentColor != 0)
    currentColor = MAX_COLOR_VALUE;
  int lastValue = 0;
  for(int i = 0;i < _thresholds.size();++i)
  {
    double xFrom = xMap.transform(lastValue);
    double xTo = xMap.transform(_thresholds[i]);
    painter->fillRect(xFrom, lower, xTo - xFrom, upper - lower,
                      QColor(currentColor, currentColor, currentColor, 100));
    lastValue = _thresholds[i];
    currentColor = MAX_COLOR_VALUE - currentColor;
  }
  int xFrom = xMap.transform(lastValue);
  int xTo = xMap.transform(MAX_COLOR_VALUE);
  painter->fillRect(xFrom, lower, xTo - xFrom, upper - lower,
                    QColor(currentColor, currentColor, currentColor, 100));
}

bool ThresholdItem::eventFilter(QObject *object, QEvent *event)
{
  if (object != plot()->canvas())
    return false;
  QwtScaleMap xMap = plot()->canvasMap(QwtPlot::xBottom);
  switch (event->type())
  {
  case QEvent::MouseButtonPress:
    {
      int currentValue =
          qBound(0,
                 (int) xMap.invTransform(((QMouseEvent*)event)->x()),
                 MAX_COLOR_VALUE);
      draggingValue = -1;
      for (int i = 0;i < _thresholds.size();++i)
        if (_thresholds[i] - currentValue >= -2 &&
            _thresholds[i] - currentValue <= 2)
        {
          draggingValue = _thresholds[i];
          break;
        }
      break;
    }
  case QEvent::MouseButtonDblClick:
    {
      int currentValue =
          qBound(0,
                 (int) xMap.invTransform(((QMouseEvent*)event)->x()),
                 MAX_COLOR_VALUE);
      draggingValue = -1;
      if (((QMouseEvent*)event)->button() == Qt::LeftButton)
      {
        int i = 0;
        for (;i < _thresholds.size();++i)
          if (_thresholds[i] >= currentValue)
          {
            _thresholds.insert(i, currentValue);
            draggingValue = currentValue;
            plot()->replot();
            emit thresholdChanged(_thresholds);
            break;
          }
        if (i == _thresholds.size())
        {
          _thresholds.push_back(currentValue);
          draggingValue = currentValue;
          plot()->replot();
          emit thresholdChanged(_thresholds);
        }
      }
      else if (((QMouseEvent*)event)->button() == Qt::RightButton)
      {
        int i = 0;
        for (;i < _thresholds.size();++i)
          if (_thresholds[i] - currentValue >= -2 &&
              _thresholds[i] - currentValue <= 2)
          {
            _thresholds.remove(i);
            plot()->replot();
            emit thresholdChanged(_thresholds);
            break;
          }
      }
      break;
    }
  case QEvent::MouseMove:
    {
      int currentValue =
          qBound(0,
                 (int) xMap.invTransform(((QMouseEvent*)event)->x()),
                 MAX_COLOR_VALUE);
      int i = 0;
      for (;i < _thresholds.size();++i)
        if (_thresholds[i] == draggingValue)
          break;
      if (i != _thresholds.size())
      {
        _thresholds[i] = currentValue;
        draggingValue = currentValue;
      }
      plot()->replot();
      emit thresholdChanged(_thresholds);
      break;
    }
  case QEvent::MouseButtonRelease:
    {
      draggingValue = -1;
      break;
    }
  default:
    break;
  }
  return false;
}
