#include "newfiledialog.h"
#include "ui_newfiledialog.h"

static MyImage::ImageTypeFlag comboMap[] = {MyImage::Colored,
                                            MyImage::Gray,
                                            MyImage::BlackAndWhite};

NewFileDialog::NewFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFileDialog)
{
  ui->setupUi(this);
}

NewFileDialog::~NewFileDialog()
{
  delete ui;
}

void NewFileDialog::on_okPushButton_clicked()
{
  MyImage::ImageTypeFlag type = comboMap[ui->typeComboBox->currentIndex()];
  int width = ui->widthSpinBox->value();
  int height = ui->heightSpinBox->value();
  emit confirmed(type, width, height);
  accept();
}

void NewFileDialog::on_cancelPushButton_clicked()
{
  reject();
}
