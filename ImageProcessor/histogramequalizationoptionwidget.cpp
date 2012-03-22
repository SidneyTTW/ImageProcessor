#include "histogramequalizationoptionwidget.h"
#include "ui_histogramequalizationoptionwidget.h"

HistogramEqualizationOptionWidget::HistogramEqualizationOptionWidget
    (ImageAlgorithm::RGBAField tunel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistogramEqualizationOptionWidget)
{
  ui->setupUi(this);
  if (tunel.testFlag(ImageAlgorithm::Field_R))
    ui->redCheckBox->setChecked(true);
  if (tunel.testFlag(ImageAlgorithm::Field_G))
    ui->greenCheckBox->setChecked(true);
  if (tunel.testFlag(ImageAlgorithm::Field_B))
    ui->blueCheckBox->setChecked(true);
}

void HistogramEqualizationOptionWidget::tryToEmit()
{
  ImageAlgorithm::RGBAField tunel = 0;
  if (ui->redCheckBox->isChecked())
    tunel |= ImageAlgorithm::Field_R;
  if (ui->greenCheckBox->isChecked())
    tunel |= ImageAlgorithm::Field_G;
  if (ui->blueCheckBox->isChecked())
    tunel |= ImageAlgorithm::Field_B;
  emit changeToTunel(tunel);
}

HistogramEqualizationOptionWidget::~HistogramEqualizationOptionWidget()
{
  delete ui;
}

void HistogramEqualizationOptionWidget::on_redCheckBox_toggled(bool checked)
{
  tryToEmit();
}

void HistogramEqualizationOptionWidget::on_greenCheckBox_toggled(bool checked)
{
  tryToEmit();
}

void HistogramEqualizationOptionWidget::on_blueCheckBox_toggled(bool checked)
{
  tryToEmit();
}

void HistogramEqualizationOptionWidget::on_confirmButton_clicked()
{
  emit confirm();
}
