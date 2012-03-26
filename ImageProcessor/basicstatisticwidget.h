#ifndef BASICSTATISTICWIDGET_H
#define BASICSTATISTICWIDGET_H

#include <QDockWidget>

#include <QImage>
#include <qwt_plot_zoomer.h>

namespace Ui {
  class BasicStatisticWidget;
}

class BasicStatisticPlot;
class ImageViewWidget;

/**
 * Class of widget bounded with a ImageViewWidget to show the statistic.
 */
class BasicStatisticWidget : public QDockWidget
{
  Q_OBJECT

public:
  /**
   * Constructor.
   */
  explicit BasicStatisticWidget(QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~BasicStatisticWidget();

  /**
   * Set the bounded image view widget.
   * Should be called only once.
   *
   * @param widget The image view widget.
   */
  void setBoundedImageView(ImageViewWidget *widget);

  /**
   * Set the image.
   *
   * @param image The image.
   */
  inline void setImage(const QImage& image)
  {
    _image = image;
    updateStatistics(true);
  }

private:
  Ui::BasicStatisticWidget *ui;

  /**
   * The plot.
   */
  BasicStatisticPlot *plot;

  /**
   * The zoomer.
   */
  QwtPlotZoomer *zoomer;

  /**
   * The image.
   */
  QImage _image;

private slots:
  /**
   * Slot to change image.
   */
  void changeImage(const QImage& image);

  /**
   * Slot to update the statistics.
   */
  void updateStatistics(bool);
};

#endif // BASICSTATISTICWIDGET_H
