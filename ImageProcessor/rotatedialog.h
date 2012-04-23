#ifndef ROTATEDIALOG_H
#define ROTATEDIALOG_H

#include <QDialog>

#include "imagealgorithm.h"

namespace Ui {
    class RotateDialog;
}

/**
 * Class of dialog to set the option of a rotate processor.
 */
class RotateDialog : public QDialog
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  explicit RotateDialog(const QImage& image, const Area& area, QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~RotateDialog();

private:
  Ui::RotateDialog *ui;

  /**
   * The image.
   */
  QImage _image;

  /**
   * Reset the preview.
   */
  void resetPreview();

  /**
   * The area to rotate.
   */
  Area _area;

signals:
  /**
   * Signal emitted when the option is confirmed.
   */
  void confirm(Area area,
               ImageAlgorithm::GeometricAlgorithmType,
               double);

private slots:
  void on_bicubicButton_toggled(bool checked);
  void on_bilinearButton_toggled(bool checked);
  void on_nearestButton_toggled(bool checked);
  void on_cancelPushButton_clicked();
  void on_okPushButton_clicked();
  void on_angleSlider_valueChanged(int value);
};

#endif // ROTATEDIALOG_H
