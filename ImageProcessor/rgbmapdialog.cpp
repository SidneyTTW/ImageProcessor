#include "rgbmapdialog.h"
#include "ui_rgbmapdialog.h"

#include "imagealgorithm.h"

RGBMapDialog::RGBMapDialog(const QImage& image, const Area& area, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RGBMapDialog),
    _image(image),
    _area(area)
{
  ui->setupUi(this);
  for (int i = 0;i < 3;++i)
    for (int j = 0;j <= MAX_COLOR_VALUE;++j)
      map[i][j] = j;
  resetPreview();
  connect(ui->plot,
          SIGNAL(optionChanged(QVector<QPointF>*)),
          SLOT(optionChanged(QVector<QPointF>*)));
}

void RGBMapDialog::resetPreview()
{
  QImage *image = ImageAlgorithm::changeRGBWithMap
                  (_image, map[0], map[1], map[2],
                   (ImageAlgorithm::RGBAField) (ImageAlgorithm::Field_R |
                                                ImageAlgorithm::Field_G |
                                                ImageAlgorithm::Field_B),
                   _area);
  if (image != NULL)
  {
    ui->imageViewerWidget->setImage(*image);
    delete image;
  }
  else
    ui->imageViewerWidget->setImage(_image);
}

void RGBMapDialog::optionChanged(QVector<QPointF> *rgbValues)
{
  for (int i = 0;i < 3;++i)
    for (int j = 0;j <= MAX_COLOR_VALUE;++j)
      map[i][j] = qRound(rgbValues[i][j].y());
  resetPreview();
}

RGBMapDialog::~RGBMapDialog()
{
  delete ui;
}

void RGBMapDialog::on_allButton_toggled(bool checked)
{
  ui->plot->setControlMode(RGBMapPlot::All);
}

void RGBMapDialog::on_redButton_toggled(bool checked)
{
  ui->plot->setControlMode(RGBMapPlot::Red);
}

void RGBMapDialog::on_greenButton_toggled(bool checked)
{
  ui->plot->setControlMode(RGBMapPlot::Green);
}

void RGBMapDialog::on_blueButton_toggled(bool checked)
{
  ui->plot->setControlMode(RGBMapPlot::Blue);
}

void RGBMapDialog::on_panButton_toggled(bool checked)
{
  ui->plot->setControlMode(RGBMapPlot::Pan);
}

void RGBMapDialog::on_okPushButton_clicked()
{
  emit confirmed((int *)map, _area);
  accept();
}

void RGBMapDialog::on_cancelPushButton_clicked()
{
  reject();
}
