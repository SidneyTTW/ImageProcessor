#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>

#include <QImage>
#include "filterprocessor.h"

class BuiltInConvolutionDialog;

namespace Ui {
    class FilterDialog;
}

/**
 * Class of dialog to set the setting of filter.
 */
class FilterDialog : public QDialog
{
  Q_OBJECT
public:
  /**
   * Indexes of the built-in type.
   */
  enum TabIndex{Median = 0, Convolution = 1};

  /**
   * Constructor.
   */
  explicit FilterDialog(const QImage& image, const Area& area, QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~FilterDialog();

private:
  Ui::FilterDialog *ui;

  /**
   * The area to filt.
   */
  Area _area;

  /**
   * The image.
   */
  QImage _image;

  /**
   * Whether the ui is changing by code.
   */
  bool changingByCode;

  /**
   * Dialog to choose built-in option.
   */
  BuiltInConvolutionDialog *builtInDialog;

  /**
   * Get the matrix value at given position.
   *
   * @param row The row.
   * @param column The column.
   */
  int matrixValueAt(int row, int column);

private slots:
  void on_cancelPushButton_clicked();
  void on_okPushButton_clicked();
  void on_builtInButton_clicked();
  void on_tabWidget_currentChanged(int index);
  void on_heightSpinBox_valueChanged(int value);
  void on_widthSpinBox_valueChanged(int value);

  /**
   * Reset the preview.
   */
  void resetPreview();

  /**
   * Slot to change the convolution option.
   */
  void convolutionConfirmed(int width,
                            int height,
                            const int *matrix,
                            int divisor,
                            int offset);

  /**
   * Slot called when the matrix changed.
   */
  void cellChanged(int,int);

signals:
  /**
   * Signal emitted when the option is confirmed.
   */
  void confirmed(FilterProcessor::FilterType type,
                 int width,
                 int height,
                 Area area,
                 QVector<int> convolutionMatrix,
                 int convolutionDivisor,
                 int convolutionOffset);
};

#endif // FILTERDIALOG_H
