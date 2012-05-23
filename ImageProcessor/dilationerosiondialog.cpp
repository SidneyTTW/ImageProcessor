#include "dilationerosiondialog.h"
#include "ui_dilationerosiondialog.h"

#include <QTableWidgetItem>
#include "imagealgorithm.h"

#define ITEM_TYPE    1001

class DilationErosionItem : public QTableWidgetItem
{
public:
  DilationErosionItem() : QTableWidgetItem(ITEM_TYPE)
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

DilationErosionDialog::DilationErosionDialog(const QImage& image,
                                             const Area& area,
                                             bool gray,
                                             QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DilationErosionDialog),
    _image(image),
    _area(area),
    _gray(gray),
    changingByCode(true)
{
  ui->setupUi(this);
  ui->table->setRowCount(3);
  ui->table->setColumnCount(3);
  for (int i = 0;i < 3;++i)
    for (int j = 0;j < 3;++j)
    {
      DilationErosionItem *item = new DilationErosionItem();
      item->setData(Qt::DisplayRole, 255);
      ui->table->setItem(i, j, item);
    }
  connect(ui->table,
          SIGNAL(cellChanged(int,int)),
          this,
          SLOT(cellChanged(int,int)));
  changingByCode = false;
  resetPreview();
}

void DilationErosionDialog::resetPreview()
{
  int width = ui->widthSpinBox->value();
  int height = ui->heightSpinBox->value();
  int center = width * height / 2;
  QImage *image = NULL;
  QVector<int> matrix;
  for (int i = 0;i < height;++i)
    for (int j = 0;j < width;++j)
      matrix.push_back(matrixValueAt(i, j));

  DilationErosionProcessor::DilationErosionType type;
  if (ui->dilationButton->isChecked())
    type = DilationErosionProcessor::Dilation;
  else if (ui->erosionButton->isChecked())
    type = DilationErosionProcessor::Erosion;
  else if (ui->openButton->isChecked())
    type = DilationErosionProcessor::Open;
  else if (ui->closeButton->isChecked())
    type = DilationErosionProcessor::Close;
  if (_gray)
  {
    switch (type)
    {
    case DilationErosionProcessor::Dilation:
      {
        ImageAlgorithm::GrayDilationFilter *filter =
            new ImageAlgorithm::GrayDilationFilter(matrix.data(), center, width, height);
        image = ImageAlgorithm::filtImage<ImageAlgorithm::GrayDilationFilter>(_image, _area, filter);
        delete filter;
        break;
      }
    case DilationErosionProcessor::Erosion:
      {
        ImageAlgorithm::GrayErosionFilter *filter =
            new ImageAlgorithm::GrayErosionFilter(matrix.data(), center, width, height);
        image = ImageAlgorithm::filtImage<ImageAlgorithm::GrayErosionFilter>(_image, _area, filter);
        delete filter;
        break;
      }
    case DilationErosionProcessor::Open:
      {
        ImageAlgorithm::GrayErosionFilter *filter =
            new ImageAlgorithm::GrayErosionFilter(matrix.data(), center, width, height);
        image = ImageAlgorithm::filtImage<ImageAlgorithm::GrayErosionFilter>(_image, _area, filter);
        delete filter;
        ImageAlgorithm::GrayDilationFilter *filter2 =
            new ImageAlgorithm::GrayDilationFilter(matrix.data(), center, width, height);
        ImageAlgorithm::filtImage<ImageAlgorithm::GrayDilationFilter>(image, _area, filter2);
        delete filter2;
        break;
      }
    case DilationErosionProcessor::Close:
      {
        ImageAlgorithm::GrayDilationFilter *filter =
            new ImageAlgorithm::GrayDilationFilter(matrix.data(), center, width, height);
        image = ImageAlgorithm::filtImage<ImageAlgorithm::GrayDilationFilter>(_image, _area, filter);
        delete filter;
        ImageAlgorithm::GrayErosionFilter *filter2 =
            new ImageAlgorithm::GrayErosionFilter(matrix.data(), center, width, height);
        ImageAlgorithm::filtImage<ImageAlgorithm::GrayErosionFilter>(image, _area, filter2);
        delete filter2;
        break;
      }
    }
  }
  else
  {
    switch (type)
    {
    case DilationErosionProcessor::Dilation:
      {
        ImageAlgorithm::DilationFilter *filter =
            new ImageAlgorithm::DilationFilter(matrix.data(), center, width, height);
        image = ImageAlgorithm::filtImage<ImageAlgorithm::DilationFilter>(_image, _area, filter);
        delete filter;
        break;
      }
    case DilationErosionProcessor::Erosion:
      {
        ImageAlgorithm::ErosionFilter *filter =
            new ImageAlgorithm::ErosionFilter(matrix.data(), center, width, height);
        image = ImageAlgorithm::filtImage<ImageAlgorithm::ErosionFilter>(_image, _area, filter);
        delete filter;
        break;
      }
    case DilationErosionProcessor::Open:
      {
        ImageAlgorithm::ErosionFilter *filter =
            new ImageAlgorithm::ErosionFilter(matrix.data(), center, width, height);
        image = ImageAlgorithm::filtImage<ImageAlgorithm::ErosionFilter>(_image, _area, filter);
        delete filter;
        ImageAlgorithm::DilationFilter *filter2 =
            new ImageAlgorithm::DilationFilter(matrix.data(), center, width, height);
        ImageAlgorithm::filtImage<ImageAlgorithm::DilationFilter>(image, _area, filter2);
        delete filter2;
        break;
      }
    case DilationErosionProcessor::Close:
      {
        ImageAlgorithm::DilationFilter *filter =
            new ImageAlgorithm::DilationFilter(matrix.data(), center, width, height);
        image = ImageAlgorithm::filtImage<ImageAlgorithm::DilationFilter>(_image, _area, filter);
        delete filter;
        ImageAlgorithm::ErosionFilter *filter2 =
            new ImageAlgorithm::ErosionFilter(matrix.data(), center, width, height);
        ImageAlgorithm::filtImage<ImageAlgorithm::ErosionFilter>(image, _area, filter2);
        delete filter2;
        break;
      }
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

void DilationErosionDialog::cellChanged(int,int)
{
  resetPreview();
}

int DilationErosionDialog::matrixValueAt(int row, int column)
{
  DilationErosionItem *item = (DilationErosionItem *)ui->table->item(row, column);
  if (item == NULL)
    return 0;
  return item->value();
}

DilationErosionDialog::~DilationErosionDialog()
{
  delete ui;
}

void DilationErosionDialog::on_dilationButton_toggled(bool checked)
{
  resetPreview();
}

void DilationErosionDialog::on_erosionButton_toggled(bool checked)
{
  resetPreview();
}

void DilationErosionDialog::on_openButton_toggled(bool checked)
{
  resetPreview();
}

void DilationErosionDialog::on_closeButton_toggled(bool checked)
{
  resetPreview();
}

void DilationErosionDialog::on_widthSpinBox_valueChanged(int value)
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
  ui->table->setColumnCount(value);
  for (int i = 0;i < ui->heightSpinBox->value();++i)
    for (int j = 0;j < value;++j)
    {
      if (ui->table->item(i, j) != NULL)
        continue;
      DilationErosionItem *item = new DilationErosionItem();
      item->setData(Qt::EditRole, QVariant(255));
      ui->table->setItem(i, j, item);
    }
  changingByCode = false;
  resetPreview();
}

void DilationErosionDialog::on_heightSpinBox_valueChanged(int value)
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
  ui->table->setRowCount(value);
  for (int i = 0;i < value;++i)
    for (int j = 0;j < ui->widthSpinBox->value();++j)
    {
      if (ui->table->item(i, j) != NULL)
        continue;
      DilationErosionItem *item = new DilationErosionItem();
      item->setData(Qt::EditRole, QVariant(255));
      ui->table->setItem(i, j, item);
    }
  changingByCode = false;
  resetPreview();
}

void DilationErosionDialog::on_okPushButton_clicked()
{
  DilationErosionProcessor::DilationErosionType type;
  if (ui->dilationButton->isChecked())
    type = DilationErosionProcessor::Dilation;
  else if (ui->erosionButton->isChecked())
    type = DilationErosionProcessor::Erosion;
  else if (ui->openButton->isChecked())
    type = DilationErosionProcessor::Open;
  else if (ui->closeButton->isChecked())
    type = DilationErosionProcessor::Close;
  int width = ui->widthSpinBox->value();
  int height = ui->heightSpinBox->value();
  int center = width * height / 2;
  QVector<int> matrix;
  for (int i = 0;i < height;++i)
    for (int j = 0;j < width;++j)
      matrix.push_back(matrixValueAt(i, j));
  emit confirmed(type, width, height, center, _gray, _area,  matrix);
  accept();
}

void DilationErosionDialog::on_cancelPushButton_clicked()
{
  reject();
}
