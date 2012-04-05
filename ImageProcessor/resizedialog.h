#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QDialog>

#include "resizeprocessor.h"

namespace Ui {
  class ResizeDialog;
}

/**
 * Class of dialog to set the option of a reize processor.
 */
class ResizeDialog : public QDialog
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  explicit ResizeDialog(const QImage& image, const Area& area, QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~ResizeDialog();

private:
  Ui::ResizeDialog *ui;

  /**
   * The image.
   */
  QImage _image;

  /**
   * Whether the things in ui is changing by code.
   */
  bool changingByCode;

  /**
   * Reset the preview.
   */
  void resetPreview();

  /**
   * The area to filt.
   */
  Area _area;

signals:
  /**
   * Signal emitted when the option is confirmed.
   */
  void confirm(ResizeProcessor::SizeType sizeType,
               Area area,
               ImageAlgorithm::ResizeAlgorithmType algorithmType,
               double width,
               double height);

private slots:
    void on_cancelPushButton_clicked();
    void on_okPushButton_clicked();
    void on_heightRateSpinBox_valueChanged(double );
    void on_widthRateSpinBox_valueChanged(double );
    void on_heightSpinBox_valueChanged(int );
    void on_widthSpinBox_valueChanged(int );
    void on_bicubicButton_toggled(bool checked);
    void on_bilinearButton_toggled(bool checked);
    void on_nearestButton_toggled(bool checked);
    void on_relativeButton_toggled(bool checked);
    void on_absoluteButton_toggled(bool checked);
};

#endif // RESIZEDIALOG_H
