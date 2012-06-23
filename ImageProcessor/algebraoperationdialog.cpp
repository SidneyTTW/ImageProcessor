#include "algebraoperationdialog.h"
#include "ui_algebraoperationdialog.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

AlgebraOperationDialog::AlgebraOperationDialog(const QImage& image,
                                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlgebraOperationDialog),
    _image(image)
{
  ui->setupUi(this);
  connect(ui->addButton, SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)));
  connect(ui->minusButton, SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)));
  connect(ui->multiplyButton, SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)));
  connect(ui->divideButton, SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)));
  resetPreview();
}

void AlgebraOperationDialog::resetPreview(ImageAlgorithm::AlgebraOperationType type)
{
  QString path = ui->fileEdit->text();
  QFileInfo fileInfo(path);
  if (fileInfo.isFile())
  {
    QImage *image =  ImageAlgorithm::algebraOperation
                     (_image,
                      QImage(path).convertToFormat(QImage::Format_ARGB32),
                      type);
    if (image != NULL)
    {
      ui->imageViewerWidget->setImage(*image);
      delete image;
    }
    else
      ui->imageViewerWidget->setImage(_image);
  }
  else
    ui->imageViewerWidget->setImage(_image);
}

AlgebraOperationDialog::~AlgebraOperationDialog()
{
  delete ui;
}

void AlgebraOperationDialog::on_fileEdit_textChanged(QString )
{
  ImageAlgorithm::AlgebraOperationType type;
  if (ui->addButton->isChecked())
    type = ImageAlgorithm::Add;
  else if (ui->minusButton->isChecked())
    type = ImageAlgorithm::Minus;
  else if (ui->multiplyButton->isChecked())
    type = ImageAlgorithm::Multiply;
  else if (ui->divideButton->isChecked())
    type = ImageAlgorithm::Divide;
  resetPreview(type);
}

void AlgebraOperationDialog::buttonToggled(bool)
{
  ImageAlgorithm::AlgebraOperationType type;
  if (ui->addButton->isChecked())
    type = ImageAlgorithm::Add;
  else if (ui->minusButton->isChecked())
    type = ImageAlgorithm::Minus;
  else if (ui->multiplyButton->isChecked())
    type = ImageAlgorithm::Multiply;
  else if (ui->divideButton->isChecked())
    type = ImageAlgorithm::Divide;
  resetPreview(type);
}

void AlgebraOperationDialog::on_okPushButton_clicked()
{
  QString path = ui->fileEdit->text();
  QFileInfo fileInfo(path);
  if (!fileInfo.isFile())
  {
    QMessageBox::critical(this, "Oops", "Please select an image file!");
    return;
  }
  ImageAlgorithm::AlgebraOperationType type;
  if (ui->addButton->isChecked())
    type = ImageAlgorithm::Add;
  else if (ui->minusButton->isChecked())
    type = ImageAlgorithm::Minus;
  else if (ui->multiplyButton->isChecked())
    type = ImageAlgorithm::Multiply;
  else if (ui->divideButton->isChecked())
    type = ImageAlgorithm::Divide;
  emit confirm(type, path);
  accept();
}

void AlgebraOperationDialog::on_cancelPushButton_clicked()
{
  reject();
}

void AlgebraOperationDialog::on_fileButton_clicked()
{
  QString path =
      QFileDialog::getOpenFileName(this,
                                   "Select an image file",
                                   tr(""),
                                   "Image (*.png *.jpg *.jpeg *.bmp *.gif *.tif)");
  ui->fileEdit->setText(path);
}
