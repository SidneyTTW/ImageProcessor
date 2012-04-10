#include "batchconvertiondialog.h"
#include "ui_batchconvertiondialog.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QProgressDialog>
#include "abstractimageprocessor.h"
#include "processchain.h"

BatchConvertionDialog::BatchConvertionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BatchConvertionDialog)
{
  ui->setupUi(this);
  model = new QStringListModel();
  ui->fileListView->setModel(model);
}

BatchConvertionDialog::~BatchConvertionDialog()
{
  delete ui;
  delete model;
}

void BatchConvertionDialog::on_setProcessorButton_clicked()
{
  QString path = QFileDialog::getOpenFileName(this,
                                              "Select a process chain file",
                                              tr(""),
                                              "Process Chain (*.pc)");
  if (!path.isEmpty())
    ui->processorLineEdit->setText(path);
}

void BatchConvertionDialog::on_targetFolderButton_clicked()
{
  QString path = QFileDialog::getExistingDirectory
                 (this, "Select a folder to store images", tr(""));
  if (!path.isEmpty())
    ui->targetFolderLineEdit->setText(path);
}

void BatchConvertionDialog::on_addFileButton_clicked()
{
  QStringList list = QFileDialog::getOpenFileNames
                     (this,
                      "Select image files",
                      tr(""),
                      "Image (*.png *.jpg *.jpeg *.bmp *.gif)");
  QStringList list2 = model->stringList();
  list2.append(list);
  model->setStringList(list2);
}

void BatchConvertionDialog::on_clearButton_clicked()
{
  model->setStringList(QStringList());
}

void BatchConvertionDialog::on_okButton_clicked()
{
  QStringList list = model->stringList();
  if (list.isEmpty() ||
      ui->processorLineEdit->text().isEmpty() ||
      ui->targetFolderLineEdit->text().isEmpty())
  {
    QMessageBox::critical(this,
                          "Information incompleted.",
                          "Please fill all the blanks.");
    return;
  }
  QList<AbstractImageProcessor *> chains =
      ProcessChain::loadProcessor(ui->processorLineEdit->text());
  if (chains.isEmpty())
  {
    QMessageBox::critical(this,
                          "Failed",
                          "Something wrong with the process chain file.");
    return;
  }
  QDir().mkpath(ui->targetFolderLineEdit->text());
  if (!QDir::setCurrent(ui->targetFolderLineEdit->text()))
  {
    QMessageBox::critical(this,
                          "Failed",
                          "Something wrong with the target folder.");
    return;
  }
  QProgressDialog progress("Processing images",
                           "Cancel",
                           0,
                           list.size(),
                           this);
  progress.setWindowTitle("Processing");
  progress.setMinimumDuration(1);
  progress.setWindowModality(Qt::WindowModal);
  QString imagePath;
  int i = 0;
  foreach (imagePath, list)
  {
    if (progress.wasCanceled())
      return;
    QImage *image = new QImage(QImage(imagePath).
                               convertToFormat(QImage::Format_ARGB32));
    AbstractImageProcessor *processor;
    foreach(processor, chains)
      if (processor != NULL)
        processor->processImage(image);
    QFile file(QFileInfo(imagePath).fileName());
    if (file.open(QFile::WriteOnly | QFile::Truncate))
      image->save(&file);
    delete image;
    file.close();
    progress.setValue(i++);
  }
  QMessageBox::information(this,
                           "Convertion completed.",
                           "Convertion completed successfully.");
  accept();
}

void BatchConvertionDialog::on_cancelButton_clicked()
{
  reject();
}
