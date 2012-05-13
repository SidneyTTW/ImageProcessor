#ifndef RGBMAPDIALOG_H
#define RGBMAPDIALOG_H

#include <QDialog>

#include <QImage>
#include "area.h"
#include "myglobal.h"

namespace Ui {
    class RGBMapDialog;
}

/**
 * Class of dialog to set the setting of filter.
 */
class RGBMapDialog : public QDialog
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  explicit RGBMapDialog(const QImage& image, const Area& area, QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~RGBMapDialog();

private slots:
  /**
   * Reset the preview.
   */
  void resetPreview();

  void on_cancelPushButton_clicked();
  void on_okPushButton_clicked();
  void on_panButton_toggled(bool checked);
  void on_blueButton_toggled(bool checked);
  void on_greenButton_toggled(bool checked);
  void on_redButton_toggled(bool checked);
  void on_allButton_toggled(bool checked);

  /**
   * Called when option changed.
   */
  void optionChanged(QVector<QPointF> *rgbValues);

private:
  Ui::RGBMapDialog *ui;

  /**
   * The map of the colors.
   */
  int map[3][MAX_COLOR_VALUE + 1];

  /**
   * The area to filt.
   */
  Area _area;

  /**
   * The image.
   */
  QImage _image;

signals:
  /**
   * Signal emitted when the option is confirmed.
   */
  void confirmed(int *map, Area area);
};

#endif // RGBMAPDIALOG_H
