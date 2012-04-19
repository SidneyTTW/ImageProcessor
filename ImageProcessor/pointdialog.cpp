#include "pointdialog.h"
#include "ui_pointdialog.h"

#include "imagealgorithm.h"

PointDialog::PointDialog(const QImage& image,
                         const Area& area,
                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PointDialog),
    _image(image),
    _area(area)
{
  ui->setupUi(this);
  resetPreview();
}

void PointDialog::resetPreview()
{
  PointProcessor::ProcessorType type = ui->linearButton->isChecked() ?
                                       PointProcessor::Linear :
                                       PointProcessor::NonLinear;
  int mapR[MAX_COLOR_VALUE + 1];
  int mapG[MAX_COLOR_VALUE + 1];
  int mapB[MAX_COLOR_VALUE + 1];
  PointProcessor::calculateRGBMap(type,
                                  ui->aSpinBox->value(),
                                  ui->bSpinBox->value(),
                                  mapR,
                                  mapG,
                                  mapB);
  QImage *image = ImageAlgorithm::changeRGBWithMap
      (_image, mapR, mapG, mapB,
       (ImageAlgorithm::RGBAField)(ImageAlgorithm::Field_R |
                                   ImageAlgorithm::Field_G |
                                   ImageAlgorithm::Field_B),
       _area);
  if (image != NULL)
  {
    ui->imageViewerWidget->setImage(*image);
    delete image;
  }
  else
    ui->imageViewerWidget->setImage(_image);

}

PointDialog::~PointDialog()
{
  delete ui;
}

void PointDialog::on_linearButton_toggled(bool)
{
  resetPreview();
}

void PointDialog::on_nonlinearButton_toggled(bool)
{
  resetPreview();
}

void PointDialog::on_aSpinBox_valueChanged(double)
{
  resetPreview();
}

void PointDialog::on_bSpinBox_valueChanged(double)
{
  resetPreview();
}

void PointDialog::on_okPushButton_clicked()
{
  PointProcessor::ProcessorType type = ui->linearButton->isChecked() ?
                                       PointProcessor::Linear :
                                       PointProcessor::NonLinear;
  emit confirm(_area, type, ui->aSpinBox->value(), ui->bSpinBox->value());
  accept();
}

void PointDialog::on_cancelPushButton_clicked()
{
  reject();
}
