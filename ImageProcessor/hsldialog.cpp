#include "hsldialog.h"
#include "ui_hsldialog.h"

#include "imagealgorithm.h"

HSLDialog::HSLDialog(const QImage& image, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HSLDialog),
    _image(image)
{
  ui->setupUi(this);
  resetPreview();
}

void HSLDialog::resetPreview()
{
  QImage *image = ImageAlgorithm::changeHSL(_image,
                                            ui->hSlider->value() / 240.0,
                                            ui->sSlider->value() / 240.0,
                                            ui->lSlider->value() / 240.0);
  ui->imageViewerWidget->setImage(*image);
  delete image;
}

HSLDialog::~HSLDialog()
{
  delete ui;
}

void HSLDialog::on_hSlider_valueChanged(int value)
{
  resetPreview();
}

void HSLDialog::on_sSlider_valueChanged(int value)
{
  resetPreview();
}

void HSLDialog::on_lSlider_valueChanged(int value)
{
  resetPreview();
}

void HSLDialog::on_okPushButton_clicked()
{
  emit confirm(ui->hSlider->value() / 240.0,
               ui->sSlider->value() / 240.0,
               ui->lSlider->value() / 240.0);
  accept();
}

void HSLDialog::on_cancelPushButton_clicked()
{
  reject();
}
