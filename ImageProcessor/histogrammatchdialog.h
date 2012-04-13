#ifndef HISTOGRAMMATCHDIALOG_H
#define HISTOGRAMMATCHDIALOG_H

#include <QDialog>

namespace Ui {
  class HistogramMatchDialog;
}

class BasicStatisticPlot;

/**
 * Class of dialog to do histogram match between two images.
 */
class HistogramMatchDialog : public QDialog
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  explicit HistogramMatchDialog(const QImage& image, QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~HistogramMatchDialog();

  /**
   * Reset the preview.
   */
  void resetPreview();

private:
  Ui::HistogramMatchDialog *ui;

  /**
   * The image.
   */
  QImage _image;

  /**
   * The statistic plot of the target image.
   */
  BasicStatisticPlot *targetPlot;

  /**
   * The statistic plot of the result image.
   */
  BasicStatisticPlot *resultPlot;

signals:
  /**
   * Signal emitted when the option is confirmed.
   */
  void confirm(QString path);

private slots:
  void on_cancelPushButton_clicked();
  void on_okPushButton_clicked();
  void on_fileEdit_textChanged(QString );
  void on_fileButton_clicked();
};

#endif // HISTOGRAMMATCHDIALOG_H
