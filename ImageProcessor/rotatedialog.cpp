#include "rotatedialog.h"
#include "ui_rotatedialog.h"

RotateDialog::RotateDialog(const QImage& image, const Area& area, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RotateDialog),
    _image(image),
    _area(area)
{
  ui->setupUi(this);
  resetPreview();
}

void RotateDialog::resetPreview()
{
  ImageAlgorithm::GeometricAlgorithmType type;
  if (ui->nearestButton->isChecked())
    type = ImageAlgorithm::NearestNeighbor;
  else if (ui->bilinearButton->isChecked())
    type = ImageAlgorithm::Bilinear;
  else
    type = ImageAlgorithm::Bicubic;
  QImage *image = ImageAlgorithm::rotate(_image,
                                         ui->angleSlider->value() * PI / 180.0,
                                         type,
                                         _area);
  ui->imageViewerWidget->setImage(*image);
  delete image;
}

RotateDialog::~RotateDialog()
{
  delete ui;
}

void RotateDialog::on_angleSlider_valueChanged(int value)
{
  resetPreview();
}

void RotateDialog::on_okPushButton_clicked()
{
  ImageAlgorithm::GeometricAlgorithmType type;
  if (ui->nearestButton->isChecked())
    type = ImageAlgorithm::NearestNeighbor;
  else if (ui->bilinearButton->isChecked())
    type = ImageAlgorithm::Bilinear;
  else
    type = ImageAlgorithm::Bicubic;
  emit confirm(_area, type, ui->angleSlider->value() * PI / 180.0);
  accept();
}

void RotateDialog::on_cancelPushButton_clicked()
{
  reject();
}
