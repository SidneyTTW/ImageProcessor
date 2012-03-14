#ifndef BATCHCONVERTIONDIALOG_H
#define BATCHCONVERTIONDIALOG_H

#include <QDialog>

namespace Ui {
  class BatchConvertionDialog;
}

class BatchConvertionDialog : public QDialog
{
  Q_OBJECT

public:
  explicit BatchConvertionDialog(QWidget *parent = 0);
  ~BatchConvertionDialog();

private:
  Ui::BatchConvertionDialog *ui;

private slots:
  void on_cancelButton_clicked();
  void on_okButton_clicked();
  void on_clearButton_clicked();
  void on_addFileButton_clicked();
  void on_targetFolderButton_clicked();
  void on_setProcessorButton_clicked();
};

#endif // BATCHCONVERTIONDIALOG_H
