#ifndef CONVOLUTIONDIALOG_H
#define CONVOLUTIONDIALOG_H

#include <QDialog>

#include "myimage.h"

namespace Ui {
  class ConvolutionDialog;
}


/**
 * Class of dialog to set the settings of convoluting an image.
 */
class ConvolutionDialog : public QDialog
{
  Q_OBJECT

public:
  /**
   * Constructor
   */
  explicit ConvolutionDialog(const MyImage& image, QWidget *parent = 0);

  /**
   * Destructor
   */
  ~ConvolutionDialog();

private:
  Ui::ConvolutionDialog *ui;

  /**
   * The original image.
   */
  QImage _image;

  /**
   * Whether it's dealing a signal, try to avoid uneccessary multiple call.
   */
  bool changing;

  /**
   * Reset the preview.
   */
  void resetPreview();

private slots:
  void on_cancelPushButton_clicked();
  void on_okPushButton_clicked();
  void on_offsetSpinBox_valueChanged(int );
  void on_divisorSpinBox_valueChanged(int );
  void on_widthSpinBox_valueChanged(int );
  void on_matrixTextEdit_textChanged();

signals:
  /**
   * Signal emitted when the configuration is confirmed.
   */
  void confirm(QVector<int> matrix,
               int matrixWidth,
               int divisor,
               int offset);
};

#endif // CONVOLUTIONDIALOG_H
