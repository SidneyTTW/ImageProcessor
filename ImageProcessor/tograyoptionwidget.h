#ifndef TOGRAYOPTIONWIDGET_H
#define TOGRAYOPTIONWIDGET_H

#include <QWidget>

#include "imagealgorithm.h"

namespace Ui {
  class ToGrayOptionWidget;
}

/**
 * Class of widget to choose gray option widget.
 */
class ToGrayOptionWidget : public QWidget
{
  Q_OBJECT
public:
  /**
   * Constructor.
   *
   * @param type Type of the algorithm.
   * @param parent The parent widget.
   */
  explicit ToGrayOptionWidget(ImageAlgorithm::ImageToGrayAlgorithmType type,
                              QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~ToGrayOptionWidget();

private:
  Ui::ToGrayOptionWidget *ui;

signals:
  /**
   * Signal emitted when the option has been changed.
   */
  void optionChanged(ImageAlgorithm::ImageToGrayAlgorithmType type);

  /**
   * Signal emitted when the option is confirmed.
   */
  void confirm();

private slots:
  /**
   * Slot should be called when any button has been .
   */
  void buttonTroggled(bool);
};

#endif // TOGRAYOPTIONWIDGET_H
