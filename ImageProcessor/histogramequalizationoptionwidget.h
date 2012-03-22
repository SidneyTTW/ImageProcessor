#ifndef HISTOGRAMEQUALIZATIONOPTIONWIDGET_H
#define HISTOGRAMEQUALIZATIONOPTIONWIDGET_H

#include <QWidget>

#include "imagealgorithm.h"

/**
 * Class of widget to choose histogram equalization option.
 */
namespace Ui {
  class HistogramEqualizationOptionWidget;
}

class HistogramEqualizationOptionWidget : public QWidget
{
  Q_OBJECT
public:
  /**
   * Constructor.
   *
   * @param tunel Tunel of the algorithm.
   * @param parent The parent widget.
   */
  explicit HistogramEqualizationOptionWidget
      (ImageAlgorithm::RGBAField tunel, QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~HistogramEqualizationOptionWidget();

private:
  Ui::HistogramEqualizationOptionWidget *ui;

  /**
   * Function which will emit the signal.
   */
  void tryToEmit();

private slots:
  void on_confirmButton_clicked();
  void on_blueCheckBox_toggled(bool checked);
  void on_greenCheckBox_toggled(bool checked);
  void on_redCheckBox_toggled(bool checked);

signals:
  /**
   * Signal emitted when the tunel is changed.
   */
  void changeToTunel(ImageAlgorithm::RGBAField tunel);

  /**
   * Signal emitted when the option is confirmed.
   */
  void confirm();
};

#endif // HISTOGRAMEQUALIZATIONOPTIONWIDGET_H
