#include "resizedialog.h"
#include "ui_resizedialog.h"

ResizeDialog::ResizeDialog(const QImage& image, const Area& area, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResizeDialog),
    _image(image),
    _area(area)
{
  ui->setupUi(this);
  changingByCode = true;
  if (area.getType() == Area::TypeRectangle || area.getType() == Area::TypeSquare)
  {
    QRect rect = area.getRectangle();
    ui->widthSpinBox->setValue(rect.width());
    ui->heightSpinBox->setValue(rect.height());
  }
  else
  {
    ui->widthSpinBox->setValue(image.width());
    ui->heightSpinBox->setValue(image.height());
  }
  changingByCode = false;
  resetPreview();
}

void ResizeDialog::resetPreview()
{
  int widthFrom = _image.width();
  int heightFrom = _image.height();
  if (_area.getType() == Area::TypeRectangle || _area.getType() == Area::TypeSquare)
  {
    QRect rect = _area.getRectangle();
    widthFrom = rect.width();
    heightFrom = rect.height();
  }
  int widthTo = ui->absoluteButton->isChecked() ?
                ui->widthSpinBox->value() :
                widthFrom * ui->widthRateSpinBox->value();
  int heightTo = ui->absoluteButton->isChecked() ?
                 ui->heightSpinBox->value() :
                 heightFrom * ui->heightRateSpinBox->value();
  ImageAlgorithm::ResizeAlgorithmType type;
  if (ui->nearestButton->isChecked())
    type = ImageAlgorithm::NearestNeighbor;
  else if (ui->bilinearButton->isChecked())
    type = ImageAlgorithm::Bilinear;
  else
    type = ImageAlgorithm::Bicubic;
  QImage *image = ImageAlgorithm::resize(_image, _area, widthTo, heightTo, type);
  ui->imageViewerWidget->setImage(*image);
  delete image;
}

ResizeDialog::~ResizeDialog()
{
  delete ui;
}

void ResizeDialog::on_absoluteButton_toggled(bool checked)
{
  ui->widthSpinBox->setEnabled(true);
  ui->heightSpinBox->setEnabled(true);
  ui->widthRateSpinBox->setEnabled(false);
  ui->heightRateSpinBox->setEnabled(false);
  resetPreview();
}

void ResizeDialog::on_relativeButton_toggled(bool checked)
{
  ui->widthRateSpinBox->setEnabled(true);
  ui->heightRateSpinBox->setEnabled(true);
  ui->widthSpinBox->setEnabled(false);
  ui->heightSpinBox->setEnabled(false);
  resetPreview();
}

void ResizeDialog::on_nearestButton_toggled(bool checked)
{
  resetPreview();
}

void ResizeDialog::on_bilinearButton_toggled(bool checked)
{
  resetPreview();
}

void ResizeDialog::on_bicubicButton_toggled(bool checked)
{
  resetPreview();
}

void ResizeDialog::on_widthSpinBox_valueChanged(int value)
{
  if (changingByCode)
    return;
  changingByCode = true;
  if (_area.getType() == Area::TypeRectangle || _area.getType() == Area::TypeSquare)
    ui->widthRateSpinBox->setValue(1.0 * value / _area.getRectangle().width());
  else
    ui->widthRateSpinBox->setValue(1.0 * value / _image.width());
  changingByCode = false;
  resetPreview();
}

void ResizeDialog::on_heightSpinBox_valueChanged(int value)
{
  if (changingByCode)
    return;
  changingByCode = true;
  if (_area.getType() == Area::TypeRectangle || _area.getType() == Area::TypeSquare)
    ui->heightRateSpinBox->setValue(1.0 * value / _area.getRectangle().height());
  else
    ui->heightRateSpinBox->setValue(1.0 * value / _image.height());
  changingByCode = false;
  resetPreview();
}

void ResizeDialog::on_widthRateSpinBox_valueChanged(double value)
{
  if (changingByCode)
    return;
  changingByCode = true;
  if (_area.getType() == Area::TypeRectangle || _area.getType() == Area::TypeSquare)
    ui->widthSpinBox->setValue(value * _area.getRectangle().width());
  else
    ui->widthSpinBox->setValue(value * _image.width());
  changingByCode = false;
  resetPreview();
}

void ResizeDialog::on_heightRateSpinBox_valueChanged(double value)
{
  if (changingByCode)
    return;
  changingByCode = true;
  if (_area.getType() == Area::TypeRectangle || _area.getType() == Area::TypeSquare)
    ui->heightSpinBox->setValue(value * _area.getRectangle().height());
  else
    ui->heightSpinBox->setValue(value * _image.height());
  changingByCode = false;
  resetPreview();
}

void ResizeDialog::on_okPushButton_clicked()
{
  ResizeProcessor::SizeType sizeType = ui->absoluteButton->isChecked() ?
                                       ResizeProcessor::Absolute :
                                       ResizeProcessor::Relative;
  ImageAlgorithm::ResizeAlgorithmType algorithmType;
  if (ui->nearestButton->isChecked())
    algorithmType = ImageAlgorithm::NearestNeighbor;
  else if (ui->bilinearButton->isChecked())
    algorithmType = ImageAlgorithm::Bilinear;
  else
    algorithmType = ImageAlgorithm::Bicubic;
  double width = ui->absoluteButton->isChecked() ?
                 ui->widthSpinBox->value() :
                 ui->widthRateSpinBox->value();
  double height = ui->absoluteButton->isChecked() ?
                  ui->heightSpinBox->value() :
                  ui->heightRateSpinBox->value();
  emit confirm(sizeType, _area, algorithmType, width, height);
  accept();
}

void ResizeDialog::on_cancelPushButton_clicked()
{
  reject();
}
