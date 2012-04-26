#include "bounddialog.h"
#include "ui_bounddialog.h"

#include "imagealgorithm.h"

BoundDialog::BoundDialog(const QImage& image, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoundDialog),
    _image(image)
{
  ui->setupUi(this);
  resetPreview();
  connect(ui->upDownCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
  connect(ui->downUpCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
  connect(ui->leftRightCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
  connect(ui->rightLeftCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
}

void BoundDialog::resetPreview()
{
  QImage *result = new QImage(_image.width(), _image.height(), QImage::Format_ARGB32);
  result->fill(qRgb(0, 0, 0));
  QImage *boundImage;
  if (ui->upDownCheckBox->isChecked())
  {
    ImageAlgorithm::ConvolutionFilter *filter =
        new ImageAlgorithm::ConvolutionFilter(BoundProcessor::UpDownConvolutionCore,
                                              3,
                                              3,
                                              1,
                                              0);
    boundImage = ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(_image, Area(), filter);
    delete filter;
    if (boundImage)
    {
      ImageAlgorithm::algebraOperation(result, *boundImage, ImageAlgorithm::Add);
      delete boundImage;
    }
  }
  if (ui->downUpCheckBox->isChecked())
  {
    ImageAlgorithm::ConvolutionFilter *filter =
        new ImageAlgorithm::ConvolutionFilter(BoundProcessor::DownUpConvolutionCore,
                                              3,
                                              3,
                                              1,
                                              0);
    boundImage = ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(_image, Area(), filter);
    delete filter;
    if (boundImage)
    {
      ImageAlgorithm::algebraOperation(result, *boundImage, ImageAlgorithm::Add);
      delete boundImage;
    }
  }
  if (ui->leftRightCheckBox->isChecked())
  {
    ImageAlgorithm::ConvolutionFilter *filter =
        new ImageAlgorithm::ConvolutionFilter(BoundProcessor::LeftRightConvolutionCore,
                                              3,
                                              3,
                                              1,
                                              0);
    boundImage = ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(_image, Area(), filter);
    delete filter;
    if (boundImage)
    {
      ImageAlgorithm::algebraOperation(result, *boundImage, ImageAlgorithm::Add);
      delete boundImage;
    }
  }
  if (ui->rightLeftCheckBox->isChecked())
  {
    ImageAlgorithm::ConvolutionFilter *filter =
        new ImageAlgorithm::ConvolutionFilter(BoundProcessor::RightLeftConvolutionCore,
                                              3,
                                              3,
                                              1,
                                              0);
    boundImage = ImageAlgorithm::filtImage<ImageAlgorithm::ConvolutionFilter>(_image, Area(), filter);
    delete filter;
    if (boundImage)
    {
      ImageAlgorithm::algebraOperation(result, *boundImage, ImageAlgorithm::Add);
      delete boundImage;
    }
  }
  ui->imageViewerWidget->setImage(*result);
  delete result;
}

void BoundDialog::toggled(bool)
{
  resetPreview();
}

BoundDialog::~BoundDialog()
{
  delete ui;
}

void BoundDialog::on_okPushButton_clicked()
{
  BoundProcessor::BoundType type = 0;
  if (ui->upDownCheckBox->isChecked())
    type |= BoundProcessor::UpDown;
  if (ui->downUpCheckBox->isChecked())
    type |= BoundProcessor::DownUp;
  if (ui->leftRightCheckBox->isChecked())
    type |= BoundProcessor::LeftRight;
  if (ui->rightLeftCheckBox->isChecked())
    type |= BoundProcessor::RightLeft;
  emit confrim(type);
  accept();
}

void BoundDialog::on_cancelPushButton_clicked()
{
  reject();
}
