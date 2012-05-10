#ifndef DILATIONEROSIONDIALOG_H
#define DILATIONEROSIONDIALOG_H

#include <QDialog>

#include <QImage>
#include "dilationerosionprocessor.h"

namespace Ui {
  class DilationErosionDialog;
}

/**
 * Class of dialog to do dilation/erosion/open/close operation.
 */
class DilationErosionDialog : public QDialog
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  explicit DilationErosionDialog(const QImage& image,
                                 const Area& area,
                                 QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~DilationErosionDialog();

private:
  Ui::DilationErosionDialog *ui;

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
   * Get the matrix value at given position.
   *
   * @param row The row.
   * @param column The column.
   */
  int matrixValueAt(int row, int column);

  /**
   * Reset the preview.
   */
  void resetPreview();

private slots:
  /**
   * Slot called when the matrix changed.
   */
  void cellChanged(int,int);

  void on_cancelPushButton_clicked();
  void on_okPushButton_clicked();
  void on_heightSpinBox_valueChanged(int );
  void on_widthSpinBox_valueChanged(int );
  void on_closeButton_toggled(bool checked);
  void on_openButton_toggled(bool checked);
  void on_erosionButton_toggled(bool checked);
  void on_dilationButton_toggled(bool checked);

signals:
  /**
   * Signal emitted when the option is confirmed.
   */
  void confirmed(DilationErosionProcessor::DilationErosionType type,
                 int width,
                 int height,
                 int center,
                 Area area,
                 QVector<int> mtrix);
};

#endif // DILATIONEROSIONDIALOG_H
