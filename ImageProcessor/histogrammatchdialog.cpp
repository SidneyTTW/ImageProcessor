#include "histogrammatchdialog.h"
#include "ui_histogrammatchdialog.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include "basicstatisticplot.h"
#include "histogrammatchprocessor.h"
#include "imagealgorithm.h"

HistogramMatchDialog::HistogramMatchDialog(const QImage& image, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistogramMatchDialog),
    _image(image)
{
  ui->setupUi(this);
  targetPlot = new BasicStatisticPlot(BasicStatistic(image.width(), image.height()));
  ui->statisticLayout->addWidget(targetPlot);
  resultPlot = new BasicStatisticPlot(BasicStatistic(image.width(), image.height()));
  ui->statisticLayout->addWidget(resultPlot);
  resetPreview();
}

void HistogramMatchDialog::resetPreview()
{
  BasicStatistic statistic1 = ImageAlgorithm::getStatistic(_image, ImageAlgorithm::Green);
  QString path = ui->fileEdit->text();
  QFileInfo fileInfo(path);
  if (fileInfo.isFile())
  {
    QImage another = QImage(path).convertToFormat(QImage::Format_ARGB32);
    BasicStatistic statistic2 = ImageAlgorithm::getStatistic(another, ImageAlgorithm::Green);
    int mapR[MAX_COLOR_VALUE + 1];
    int mapG[MAX_COLOR_VALUE + 1];
    int mapB[MAX_COLOR_VALUE + 1];
    HistogramMatchProcessor::calculateRGBMap(statistic1, statistic2, mapR, mapG, mapB);
    QImage *result = ImageAlgorithm::changeRGBWithMap
                     (_image, mapR, mapG, mapB,
                      (ImageAlgorithm::RGBAField)(ImageAlgorithm::Field_R |
                                                  ImageAlgorithm::Field_G |
                                                  ImageAlgorithm::Field_B));
    targetPlot->setStatistic(statistic2);
    if (result != NULL)
    {
      ui->imageViewerWidget->setImage(*result);
      resultPlot->setStatistic(ImageAlgorithm::getStatistic
                               (*result, ImageAlgorithm::Green));
      delete result;
    }
    else
    {
      ui->imageViewerWidget->setImage(_image);
      resultPlot->setStatistic(statistic1);
    }
  }
  else
  {
    ui->imageViewerWidget->setImage(_image);
    targetPlot->setStatistic(BasicStatistic(_image.width(), _image.height()));
    resultPlot->setStatistic(statistic1);
  }
}

HistogramMatchDialog::~HistogramMatchDialog()
{
  delete ui;
  delete targetPlot;
  delete resultPlot;
}

void HistogramMatchDialog::on_fileButton_clicked()
{
  QString path =
      QFileDialog::getOpenFileName(this,
                                   "Select an image file",
                                   tr(""),
                                   "Image (*.png *.jpg *.jpeg *.bmp *.gif)");
  ui->fileEdit->setText(path);
}

void HistogramMatchDialog::on_fileEdit_textChanged(QString )
{
  resetPreview();
}

void HistogramMatchDialog::on_okPushButton_clicked()
{
  QString path = ui->fileEdit->text();
  QFileInfo fileInfo(path);
  if (!fileInfo.isFile())
  {
    QMessageBox::critical(this, "Oops", "Please select an image file!");
    return;
  }
  emit confirm(path);
  accept();
}

void HistogramMatchDialog::on_cancelPushButton_clicked()
{
  reject();
}
