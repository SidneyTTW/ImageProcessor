#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>

#include "myimage.h"

namespace Ui {
  class NewFileDialog;
}

/**
 * Class of dialog to let the user create a new image file.
 */
class NewFileDialog : public QDialog
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  explicit NewFileDialog(QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~NewFileDialog();

private:
  Ui::NewFileDialog *ui;

private slots:
  void on_cancelPushButton_clicked();
  void on_okPushButton_clicked();

signals:
  /**
   * Signal emitted when the option is confirmed.
   */
  void confirmed(MyImage::ImageTypeFlag type, int width, int height);
};

#endif // NEWFILEDIALOG_H
