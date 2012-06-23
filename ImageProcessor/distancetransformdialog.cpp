#include "distancetransformdialog.h"
#include "ui_distancetransformdialog.h"

DistanceTransformDialog::DistanceTransformDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DistanceTransformDialog)
{
  ui->setupUi(this);
}

DistanceTransformDialog::~DistanceTransformDialog()
{
  delete ui;
}

void DistanceTransformDialog::on_okPushButton_clicked()
{
  ImageAlgorithm::DistanceTransformType type;
  if (ui->geometricDistanceButton->isChecked())
    type = ImageAlgorithm::GeometricDistance;
  else if (ui->blockDistanceButton->isChecked())
    type = ImageAlgorithm::BlockDistance;
  else if (ui->chessboardDistanceButton->isChecked())
    type = ImageAlgorithm::ChessboardDistance;
  int size = ui->spinBox->value();
  emit confirm(type, size);
  accept();
}

void DistanceTransformDialog::on_cancelPushButton_clicked()
{
  reject();
}
