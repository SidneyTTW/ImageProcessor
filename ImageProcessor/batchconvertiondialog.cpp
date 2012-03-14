#include "batchconvertiondialog.h"
#include "ui_batchconvertiondialog.h"

BatchConvertionDialog::BatchConvertionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BatchConvertionDialog)
{
  ui->setupUi(this);
}

BatchConvertionDialog::~BatchConvertionDialog()
{
  delete ui;
}

void BatchConvertionDialog::on_setProcessorButton_clicked()
{

}

void BatchConvertionDialog::on_targetFolderButton_clicked()
{

}

void BatchConvertionDialog::on_addFileButton_clicked()
{

}

void BatchConvertionDialog::on_clearButton_clicked()
{

}

void BatchConvertionDialog::on_okButton_clicked()
{

}

void BatchConvertionDialog::on_cancelButton_clicked()
{

}
