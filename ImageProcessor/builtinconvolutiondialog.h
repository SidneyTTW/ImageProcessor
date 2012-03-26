#ifndef BUILTINCONVOLUTIONDIALOG_H
#define BUILTINCONVOLUTIONDIALOG_H

#include <QDialog>

#include <QVector>

namespace Ui {
  class BuiltInConvolutionDialog;
}

/**
 * Class of dialog to choose the built in convolution core.
 */
class BuiltInConvolutionDialog : public QDialog
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  explicit BuiltInConvolutionDialog(QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~BuiltInConvolutionDialog();

private:
  Ui::BuiltInConvolutionDialog *ui;

  int *gaussMatrix;

  /**
   * Disenable all ui.
   */
  void disenableAll();

  /**
   * Width.
   */
  int width;

  /**
   * Height.
   */
  int height;

  /**
   * Matrix.
   */
  const int *matrix;

  /**
   * Divisor.
   */
  int divisor;

  /**
   * Offset.
   */
  int offset;

private slots:
  void on_cancelPushButton_clicked();
  void on_okPushButton_clicked();

  /**
   * Reset all the ui according to roberts operator.
   */
  void resetWithRoberts(bool);

  /**
   * Reset all the ui according to prewitt operator.
   */
  void resetWithPrewitt(bool);

  /**
   * Reset all the ui according to sobel operator.
   */
  void resetWithSobel(bool);

  /**
   * Reset all the ui according to gauss filter.
   */
  void resetWithGauss(bool);

  /**
   * Reset all the ui according to gauss filter.
   */
  void resetWithGauss(int);

  /**
   * Reset all the ui according to gauss filter.
   */
  void resetWithGauss(double);

  /**
   * Reset all the ui according to mean filter.
   */
  void resetWithMean(bool);

  /**
   * Reset all the ui according to mean filter.
   */
  void resetWithMean(int);

signals:
  /**
   * Signal emitted when the option is confirmed.
   */
  void confirmed(int width,
                 int height,
                 const int *matrix,
                 int divisor,
                 int offset);
};

#endif // BUILTINCONVOLUTIONDIALOG_H
