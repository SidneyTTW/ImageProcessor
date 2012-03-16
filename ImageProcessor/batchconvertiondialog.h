#ifndef BATCHCONVERTIONDIALOG_H
#define BATCHCONVERTIONDIALOG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
  class BatchConvertionDialog;
}

/**
 * Class of dialog to convert lots of images with the saved process chain in a
 * time.
 */
class BatchConvertionDialog : public QDialog
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  explicit BatchConvertionDialog(QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~BatchConvertionDialog();

private:
  Ui::BatchConvertionDialog *ui;

  /**
   * The model.
   */
  QStringListModel *model;

private slots:
  void on_cancelButton_clicked();
  void on_okButton_clicked();
  void on_clearButton_clicked();
  void on_addFileButton_clicked();
  void on_targetFolderButton_clicked();
  void on_setProcessorButton_clicked();
};

#endif // BATCHCONVERTIONDIALOG_H
