#ifndef POINTDIALOG_H
#define POINTDIALOG_H

#include <QDialog>

#include "pointprocessor.h"

namespace Ui {
    class PointDialog;
}

/**
 * Class of dialog to set the settings of point operate.
 */
class PointDialog : public QDialog
{
  Q_OBJECT

public:
  /**
   * Constructor
   */
  explicit PointDialog(const QImage& image,
                       const Area& area,
                       QWidget *parent = 0);

  /**
   * Destructor
   */
  ~PointDialog();

  /**
   * Reset the preview.
   */
  void resetPreview();

private:
  Ui::PointDialog *ui;

  /**
   * The original image.
   */
  QImage _image;

  /**
   * The area to change.
   */
  Area _area;

private slots:
  void on_cancelPushButton_clicked();
  void on_okPushButton_clicked();
  void on_bSpinBox_valueChanged(double);
  void on_aSpinBox_valueChanged(double);
  void on_nonlinearButton_toggled(bool);
  void on_linearButton_toggled(bool);

signals:
  /**
   * Signal emitted when the configuration is confirmed.
   */
  void confirm(Area area,
               PointProcessor::ProcessorType type,
               double para1,
               double para2);
};

#endif // POINTDIALOG_H
