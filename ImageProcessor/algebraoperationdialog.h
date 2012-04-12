#ifndef ALGEBRAOPERATIONDIALOG_H
#define ALGEBRAOPERATIONDIALOG_H

#include <QDialog>

#include "imagealgorithm.h"

namespace Ui {
  class AlgebraOperationDialog;
}

/**
 * Class of dialog to set the option of a algebra processor.
 */
class AlgebraOperationDialog : public QDialog
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  explicit AlgebraOperationDialog(const QImage& image, QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~AlgebraOperationDialog();

  /**
   * Reset the preview.
   */
  void resetPreview(ImageAlgorithm::AlgebraOperationType type = ImageAlgorithm::Add);

private:
  Ui::AlgebraOperationDialog *ui;

  /**
   * The image.
   */
  QImage _image;

signals:
  /**
   * Signal emitted when the option is confirmed.
   */
  void confirm(ImageAlgorithm::AlgebraOperationType type,
               QString path);

private slots:
  void on_fileButton_clicked();
  void on_cancelPushButton_clicked();
  void on_okPushButton_clicked();
  void on_fileEdit_textChanged(QString );

  /**
   * Slot called when any of the button toggled.
   */
  void buttonToggled(bool);
};

#endif // ALGEBRAOPERATIONDIALOG_H
