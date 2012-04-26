#ifndef BOUNDDIALOG_H
#define BOUNDDIALOG_H

#include <QDialog>

#include "boundprocessor.h"

namespace Ui {
  class BoundDialog;
}

/**
 * Class of dialog to set the setting of bound processor.
 */
class BoundDialog : public QDialog
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  explicit BoundDialog(const QImage& image, QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~BoundDialog();

private:
  Ui::BoundDialog *ui;

  /**
   * The image.
   */
  QImage _image;

  /**
   * Reset the preview.
   */
  void resetPreview();

private slots:
  void on_cancelPushButton_clicked();
  void on_okPushButton_clicked();

  /**
   * Slot called when any of the check box is toggled.
   */
  void toggled(bool);

signals:
  /**
   * Signal emitted when the option is confirmed.
   */
  void confrim(BoundProcessor::BoundType type);
};

#endif // BOUNDDIALOG_H
