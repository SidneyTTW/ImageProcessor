#include "filterdialog.h"
#include "ui_filterdialog.h"

#include <QTableWidgetItem>
#include "builtinconvolutiondialog.h"
#include "imagealgorithm.h"

#define ITEM_TYPE    1000

class Item : public QTableWidgetItem
{
public:
  Item() : QTableWidgetItem(ITEM_TYPE)
  {
    setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem::setData(Qt::DisplayRole, QVariant(0));
    QTableWidgetItem::setData(Qt::EditRole, QVariant(0));
  }

  int type() const
  {return ITEM_TYPE;}

  inline int value() const
  {return data(Qt::DisplayRole).toInt();}

  virtual QVariant data(int role) const
  {
    return QTableWidgetItem::data(role);
  }

  virtual void setData(int role, const QVariant &value)
  {
    if (role != Qt::DisplayRole && role != Qt::EditRole)
      QTableWidgetItem::setData(role, value);
    else
    {
      switch (value.type())
      {
      case QVariant::Int:
      case QVariant::Double:
        QTableWidgetItem::setData(Qt::DisplayRole, QVariant(value.toInt()));
        QTableWidgetItem::setData(Qt::EditRole, QVariant(value.toInt()));
        break;
      case QVariant::String:
        {
          bool success = false;
          double v = value.toString().toInt(&success);
          if (success)
          {
            QTableWidgetItem::setData(Qt::DisplayRole, QVariant(v));
            QTableWidgetItem::setData(Qt::EditRole, QVariant(v));
          }
          else
          {
            QTableWidgetItem::setData(Qt::DisplayRole, QVariant(0));
            QTableWidgetItem::setData(Qt::EditRole, QVariant(0));
          }
          break;
        }
      default:
        break;
      }
    }
  }
};

FilterDialog::FilterDialog(const QImage& image, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog),
    _image(image),
    changingByCode(false),
    builtInDialog(NULL)
{
  ui->setupUi(this);
  changingByCode = true;
  ui->convolutionTable->setRowCount(3);
  ui->convolutionTable->setColumnCount(3);
  for (int i = 0;i < 3;++i)
    for (int j = 0;j < 3;++j)
    {
      Item *item = new Item();
      ui->convolutionTable->setItem(i, j, item);
    }
  changingByCode = false;
  connect(ui->divisorSpinBox,
          SIGNAL(editingFinished()),
          this,
          SLOT(resetPreview()));
  connect(ui->offsetSpinBox,
          SIGNAL(editingFinished()),
          this,
          SLOT(resetPreview()));
  connect(ui->convolutionTable,
          SIGNAL(cellChanged(int,int)),
          this,
          SLOT(cellChanged(int,int)));
  resetPreview();
}

int FilterDialog::matrixValueAt(int row, int column)
{
  Item *item = (Item *)ui->convolutionTable->item(row, column);
  if (item == NULL)
    return 0;
  return item->value();
}

void FilterDialog::resetPreview()
{
  int width = ui->widthSpinBox->value();
  int height = ui->heightSpinBox->value();
  QImage *image = NULL;
  switch (ui->tabWidget->currentIndex())
  {
  case Median:
    {
      ImageAlgorithm::MedianFilter *filter =
          new ImageAlgorithm::MedianFilter(width, height);
      image = filter->filt(_image);
      delete filter;
      break;
    }
  case Convolution:
    {
      int divisor = ui->divisorSpinBox->value();
      int offset = ui->offsetSpinBox->value();
      QVector<int> matrix;
      for (int i = 0;i < height;++i)
        for (int j = 0;j < width;++j)
          matrix.push_back(matrixValueAt(i, j));
      ImageAlgorithm::ConvolutionFilter *filter =
          new ImageAlgorithm::ConvolutionFilter(matrix.data(),
                                                width,
                                                height,
                                                divisor,
                                                offset);
      image = filter->filt(_image);
      delete filter;
      break;
    }
  }
  if (image != NULL)
  {
    ui->imageViewerWidget->setImage(*image);
    delete image;
  }
  else
    ui->imageViewerWidget->setImage(_image);
}

FilterDialog::~FilterDialog()
{
  delete ui;
  if (builtInDialog != NULL)
    delete builtInDialog;
}

void FilterDialog::on_widthSpinBox_valueChanged(int value)
{
  if (changingByCode)
    return;
  if (value % 2 == 0)
  {
    value += 1;
    changingByCode = true;
    ui->widthSpinBox->setValue(value);
    changingByCode = false;
  }
  changingByCode = true;
  ui->convolutionTable->setColumnCount(value);
  for (int i = 0;i < ui->heightSpinBox->value();++i)
    for (int j = 0;j < value;++j)
    {
      if (ui->convolutionTable->item(i, j) != NULL)
        continue;
      Item *item = new Item();
      item->setData(Qt::EditRole, QVariant(0));
      ui->convolutionTable->setItem(i, j, item);
    }
  changingByCode = false;
  resetPreview();
}

void FilterDialog::on_heightSpinBox_valueChanged(int value)
{
  if (changingByCode)
    return;
  if (value % 2 == 0)
  {
    value += 1;
    changingByCode = true;
    ui->heightSpinBox->setValue(value);
    changingByCode = false;
  }
  changingByCode = true;
  ui->convolutionTable->setRowCount(value);
  for (int i = 0;i < value;++i)
    for (int j = 0;j < ui->widthSpinBox->value();++j)
    {
      if (ui->convolutionTable->item(i, j) != NULL)
        continue;
      Item *item = new Item();
      ui->convolutionTable->setItem(i, j, item);
    }
  changingByCode = false;
  resetPreview();
}

void FilterDialog::on_tabWidget_currentChanged(int index)
{
  resetPreview();
}

void FilterDialog::on_builtInButton_clicked()
{
  if (builtInDialog == NULL)
  {
    builtInDialog = new BuiltInConvolutionDialog(this);
    connect(builtInDialog,
            SIGNAL(confirmed(int,int,const int*,int,int)),
            this,
            SLOT(convolutionConfirmed(int,int,const int*,int,int)));
  }
  builtInDialog->exec();
}

void FilterDialog::convolutionConfirmed(int width,
                                        int height,
                                        const int *matrix,
                                        int divisor,
                                        int offset)
{
  changingByCode = true;
  ui->widthSpinBox->setValue(width);
  ui->heightSpinBox->setValue(height);
  ui->divisorSpinBox->setValue(divisor);
  ui->offsetSpinBox->setValue(offset);
  ui->convolutionTable->setRowCount(height);
  ui->convolutionTable->setColumnCount(width);
  for (int i = 0;i < height;++i)
    for (int j = 0;j < width;++j)
    {
      Item *item = new Item();
      item->setData(Qt::EditRole, QVariant(matrix[i * width + j]));
      ui->convolutionTable->setItem(i, j, item);
    }
  changingByCode = false;
  resetPreview();
}

void FilterDialog::cellChanged(int,int)
{
  if (changingByCode)
    return;
  resetPreview();
}

void FilterDialog::on_okPushButton_clicked()
{
  FilterProcessor::FilterType type = ui->tabWidget->currentIndex() == Median ?
                                     FilterProcessor::Midian :
                                     FilterProcessor::Convolution;
  int width = ui->widthSpinBox->value();
  int height = ui->heightSpinBox->value();
  QVector<int> matrix;
  if (ui->tabWidget->currentIndex() == Convolution)
    for (int i = 0;i < height;++i)
      for (int j = 0;j < width;++j)
        matrix.push_back(matrixValueAt(i, j));
  int divisor = ui->divisorSpinBox->value();
  int offset = ui->offsetSpinBox->value();
  emit confirmed(type, width, height, matrix, divisor, offset);
  accept();
}

void FilterDialog::on_cancelPushButton_clicked()
{
  reject();
}
