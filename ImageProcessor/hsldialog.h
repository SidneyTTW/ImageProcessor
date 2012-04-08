#ifndef HSLDIALOG_H
#define HSLDIALOG_H

#include <QDialog>

namespace Ui {
    class HSLDialog;
}

/**
 * Class of dialog to change the HSL.
 */
class HSLDialog : public QDialog
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  explicit HSLDialog(const QImage& image, QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~HSLDialog();

private:
  Ui::HSLDialog *ui;

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
    void on_lSlider_valueChanged(int value);
    void on_sSlider_valueChanged(int value);
    void on_hSlider_valueChanged(int value);

signals:
  /**
   * Signal emitted when the option is confirmed.
   */
  void confirm(double hOffset,
               double sOffset,
               double lOffset);
};

#endif // HSLDIALOG_H
