#ifndef DISTANCETRANSFORMDIALOG_H
#define DISTANCETRANSFORMDIALOG_H

#include <QDialog>

#include "imagealgorithm.h"

namespace Ui {
  class DistanceTransformDialog;
}

class DistanceTransformDialog : public QDialog
{
  Q_OBJECT
public:
  explicit DistanceTransformDialog(QWidget *parent = 0);
  ~DistanceTransformDialog();

private:
  Ui::DistanceTransformDialog *ui;

private slots:
  void on_cancelPushButton_clicked();
  void on_okPushButton_clicked();

signals:
  void confirm(ImageAlgorithm::DistanceTransformType type, int size);
};

#endif // DISTANCETRANSFORMDIALOG_H
