#ifndef TOBLACKANDWHITEDIALOG_H
#define TOBLACKANDWHITEDIALOG_H

#include <QDialog>

#include <QVector>
#include <qwt_plot_marker.h>
#include "myimage.h"
#include "toblackandwhiteprocessor.h"

class BasicStatisticPlot;
class ThresholdItem;
class XPlotPicker;

namespace Ui {
    class ToBlackAndWhiteDialog;
}

/**
 * Class of dialog to set the settings of converting an image to black and
 * white one.
 */
class ToBlackAndWhiteDialog : public QDialog
{
  Q_OBJECT
public:
  /**
   * Constructor
   */
  explicit ToBlackAndWhiteDialog(const MyImage& image, QWidget *parent = 0);

  /**
   * Destructor
   */
  ~ToBlackAndWhiteDialog();

private:
  Ui::ToBlackAndWhiteDialog *ui;

  /**
   * Whether it's dealing a signal, try to avoid uneccessary multiple call.
   */
  bool changing;

  /**
   * Whether to use a single threshold.
   */
  bool single;

  /**
   * Whether to start from black.
   */
  bool black;

  /**
   * The single threshold.
   */
  int singleThreshold;

  /**
   * The multiple thresholds.
   */
  QVector<int> _thresholds;

  /**
   * The item to show the thresholds.
   */
  ThresholdItem *thresholdItem;

  /**
   * The plot picker to modify position in x axis.
   */
  XPlotPicker *xPlotPicker;

  /**
   * The marker.
   */
  QwtPlotMarker *marker;

  /**
   * The plot.
   */
  BasicStatisticPlot *plot;

  /**
   * The original image.
   */
  QImage _image;

  /**
   * Reset the preview.
   */
  void resetPreview();

private slots:
  void on_cancelPushButton_clicked();
  void on_okPushButton_clicked();
  void on_maxEntropyButton_toggled(bool checked);
  void on_OTSUButton_toggled(bool checked);
  void on_customButton_toggled(bool checked);

  /**
   * Slot to change to single threshold.
   */
  void changeToSingle(bool selected);

  /**
   * Slot to change to multiple threshold.
   */
  void changeToMultiple(bool selected);

  /**
   * Slot to change single threshold.
   */
  void singleChanged(int x);

  /**
   * Slot to change multiple thresholds.
   */
  void multipleTextChanged(QString text);

  /**
   * Slot to change multiple thresholds.
   */
  void multipleChanged(QVector<int> thresholds);

  /**
   * Slot to change start color to black.
   */
  void startWithBlack(bool selected);

  /**
   * Slot to change start color to white.
   */
  void startWithWhite(bool selected);

signals:
  /**
   * Signal emitted when the configuration is confirmed.
   */
  void confirm(ToBlackAndWhiteProcessor::ThresholdType type,
               int startColor,
               QVector<int> thresholds);
};

#endif // TOBLACKANDWHITEDIALOG_H
