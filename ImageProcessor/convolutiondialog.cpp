#include "convolutiondialog.h"
#include "ui_convolutiondialog.h"

#include "imagealgorithm.h"

ConvolutionDialog::ConvolutionDialog(const MyImage& image, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConvolutionDialog)
{
  _image = image.getImage();
  ui->setupUi(this);
  changing = false;
  resetPreview();
}

ConvolutionDialog::~ConvolutionDialog()
{
  delete ui;
}

void ConvolutionDialog::resetPreview()
{
  QString text = ui->matrixTextEdit->toPlainText();
  QVector<int> matrix;
  QStringList list = text.split(QRegExp("\\s+"), QString::SkipEmptyParts);
  QString str;
  foreach (str, list)
    matrix.push_back(str.toInt());
  QImage *image = ImageAlgorithm::convolution(_image,
                                              matrix,
                                              ui->widthSpinBox->value(),
                                              ui->divisorSpinBox->value(),
                                              ui->offsetSpinBox->value());
  if (image != NULL)
  {
    ui->imageViewerWidget->setImage(*image);
    delete image;
  }
  else
    ui->imageViewerWidget->setImage(_image);
}

void ConvolutionDialog::on_matrixTextEdit_textChanged()
{
  QString text = ui->matrixTextEdit->toPlainText();
  if (changing)
    return;
  QString result;
  bool needToChange = false;
  for (int i = 0;i < text.size();++i)
    switch (text[i].toAscii())
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case ' ':
    case '-':
    case '\n':
      result += text[i];
      break;
    default:
      needToChange = true;
      break;
    }
  if (needToChange)
  {
    changing = true;
    ui->matrixTextEdit->setPlainText(result);
    changing = false;
  }

  resetPreview();
}

void ConvolutionDialog::on_widthSpinBox_valueChanged(int )
{
  resetPreview();
}

void ConvolutionDialog::on_divisorSpinBox_valueChanged(int )
{
  resetPreview();
}

void ConvolutionDialog::on_offsetSpinBox_valueChanged(int )
{
  resetPreview();
}

void ConvolutionDialog::on_okPushButton_clicked()
{
  QString text = ui->matrixTextEdit->toPlainText();
  QVector<int> matrix;
  QStringList list = text.split(QRegExp("\\s+"), QString::SkipEmptyParts);
  QString str;
  foreach (str, list)
    matrix.push_back(str.toInt());
  emit confirm(matrix,
               ui->widthSpinBox->value(),
               ui->divisorSpinBox->value(),
               ui->offsetSpinBox->value());
  accept();
}

void ConvolutionDialog::on_cancelPushButton_clicked()
{
  reject();
}
