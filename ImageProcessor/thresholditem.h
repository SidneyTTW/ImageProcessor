#ifndef THRESHOLDITEM_H
#define THRESHOLDITEM_H

#include <QVector>
#include <qwt_plot.h>

/**
 * Class of item to show the threshold on a plot.
 */
class ThresholdItem : public QObject, public QwtPlotItem
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  ThresholdItem();


  virtual int rtti() const;
  virtual void draw(QPainter *painter,
      const QwtScaleMap &xMap, const QwtScaleMap &yMap,
      const QRectF &rect) const;

  /**
   * @param threshold The thresholds.
   */
  inline void setThresholds(const QVector<int>& thresholds)
  {
    _thresholds = thresholds;
    plot()->replot();
  }

  /**
   * @param threshold The thresholds.
   */
  inline QVector<int> getThresholds()
  {
    return _thresholds;
  }

  /**
   * @param startColor The start color.
   */
  inline void setStartColor(int startColor)
  {
    _startColor = startColor;
    plot()->replot();
  }

protected:
  /**
   * Filter the events.
   */
  virtual bool eventFilter(QObject *object, QEvent *event);

private:
  /**
   * The thresholds.
   */
  QVector<int> _thresholds;

  /**
   * The start color.
   */
  int _startColor;

  /**
   * The dragging value.
   */
  int draggingValue;

signals:
  /**
   * Signal emitted when the threshold is changed.
   */
  void thresholdChanged(QVector<int> thresholds);
};

#endif // THRESHOLDITEM_H
