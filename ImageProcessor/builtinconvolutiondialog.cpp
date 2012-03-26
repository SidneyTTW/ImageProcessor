#include "builtinconvolutiondialog.h"
#include "ui_builtinconvolutiondialog.h"

#include "imagealgorithm.h"

static const int HORIZONTAL_SOBEL[] = {-1, -2, -1,
                                        0,  0,  0,
                                        1,  2,  1};

static const int VERTICAL_SOBEL[] = {-1,  0,  1,
                                     -2,  0,  2,
                                     -1,  0,  1};

static const int HORIZONTAL_PREWITT[] = {-1, -1, -1,
                                          0,  0,  0,
                                          1,  1,  1};

static const int VERTICAL_PREWITT[] = {-1,  0,  1,
                                       -1,  0,  1,
                                       -1,  0,  1};

static const int MAIN_ROBERTS[] = { 0,  0,  0,
                                    0, -1,  0,
                                    0,  0,  1};

static const int SUB_ROBERTS[] = { 0,  0,  0,
                                   0,  0, -1,
                                   0,  1,  0};

static const int MEAN[] = { 1,  1,  1,
                            1,  1,  1,
                            1,  1,  1};

BuiltInConvolutionDialog::BuiltInConvolutionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuiltInConvolutionDialog)
{
  ui->setupUi(this);
  connect(ui->sobelButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(resetWithSobel(bool)));
  connect(ui->sobelVerticalButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(resetWithSobel(bool)));
  connect(ui->sobelHorizontalButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(resetWithSobel(bool)));
  connect(ui->prewittButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(resetWithPrewitt(bool)));
  connect(ui->prewittHorizontalButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(resetWithPrewitt(bool)));
  connect(ui->prewittVerticalButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(resetWithPrewitt(bool)));
  connect(ui->robertsButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(resetWithRoberts(bool)));
  connect(ui->robertsMainButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(resetWithRoberts(bool)));
  connect(ui->robertsSubButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(resetWithRoberts(bool)));
  connect(ui->gaussButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(resetWithGauss(bool)));
  connect(ui->gaussSizeSpinBox,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(resetWithGauss(int)));
  connect(ui->gaussSegmaSpinBox,
          SIGNAL(valueChanged(double)),
          this,
          SLOT(resetWithGauss(double)));
  connect(ui->meanButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(resetWithMean(bool)));
  connect(ui->meanXRadiusSpinBox,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(resetWithMean(int)));
  connect(ui->meanYRadiusSpinBox,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(resetWithMean(int)));
  gaussMatrix = NULL;
  resetWithSobel(true);
}

void BuiltInConvolutionDialog::disenableAll()
{
  ui->sobelHorizontalButton->setEnabled(false);
  ui->sobelVerticalButton->setEnabled(false);
  ui->prewittHorizontalButton->setEnabled(false);
  ui->prewittVerticalButton->setEnabled(false);
  ui->robertsMainButton->setEnabled(false);
  ui->robertsSubButton->setEnabled(false);
  ui->gaussSizeSpinBox->setEnabled(false);
  ui->gaussSegmaSpinBox->setEnabled(false);
  ui->meanXRadiusSpinBox->setEnabled(false);
  ui->meanYRadiusSpinBox->setEnabled(false);
}

void BuiltInConvolutionDialog::resetWithSobel(bool)
{
  disenableAll();
  ui->sobelHorizontalButton->setEnabled(true);
  ui->sobelVerticalButton->setEnabled(true);
  ui->sizeLabel->setText("3x3");
  width = 3;
  height = 3;
  if (ui->sobelHorizontalButton->isChecked())
    matrix = HORIZONTAL_SOBEL;
  else
    matrix = VERTICAL_SOBEL;
  divisor = 1;
  offset = 0;
}

void BuiltInConvolutionDialog::resetWithPrewitt(bool)
{
  disenableAll();
  ui->prewittHorizontalButton->setEnabled(true);
  ui->prewittVerticalButton->setEnabled(true);
  ui->sizeLabel->setText("3x3");
  width = 3;
  height = 3;
  if (ui->prewittHorizontalButton->isChecked())
    matrix = HORIZONTAL_PREWITT;
  else
    matrix = VERTICAL_PREWITT;
  divisor = 1;
  offset = 0;
}

void BuiltInConvolutionDialog::resetWithRoberts(bool)
{
  disenableAll();
  ui->robertsMainButton->setEnabled(true);
  ui->robertsSubButton->setEnabled(true);
  ui->sizeLabel->setText("3x3");
  width = 3;
  height = 3;
  if (ui->robertsMainButton->isChecked())
    matrix = MAIN_ROBERTS;
  else
    matrix = SUB_ROBERTS;
  divisor = 1;
  offset = 0;
}

void BuiltInConvolutionDialog::resetWithGauss(bool)
{
  disenableAll();
  ui->gaussSizeSpinBox->setEnabled(true);
  ui->gaussSegmaSpinBox->setEnabled(true);
  width = ui->gaussSizeSpinBox->value() * 2 + 1;
  height = width;
  ui->sizeLabel->setText(tr("%1x%2").arg(width).arg(width));
  if (gaussMatrix != NULL)
    delete [] gaussMatrix;
  gaussMatrix = new int[width * width];
  ImageAlgorithm::gaussCore(ui->gaussSizeSpinBox->value(),
                            ui->gaussSegmaSpinBox->value(),
                            gaussMatrix);
  divisor = 0;
  for (int i = 0;i < width * width;++i)
    divisor += gaussMatrix[i];
  offset = 0;
  matrix = gaussMatrix;
}

void BuiltInConvolutionDialog::resetWithGauss(int)
{
  resetWithGauss(true);
}

void BuiltInConvolutionDialog::resetWithGauss(double)
{
  resetWithGauss(true);
}

void BuiltInConvolutionDialog::resetWithMean(bool)
{
  disenableAll();
  ui->meanXRadiusSpinBox->setEnabled(true);
  ui->meanYRadiusSpinBox->setEnabled(true);
  width = ui->meanXRadiusSpinBox->value() * 2 + 1;
  height = ui->meanYRadiusSpinBox->value() * 2 + 1;
  ui->sizeLabel->setText(tr("%1x%2").arg(height).arg(width));
  matrix = MEAN;
  divisor = 9;
  offset = 0;
}

void BuiltInConvolutionDialog::resetWithMean(int)
{
  resetWithMean(true);
}

BuiltInConvolutionDialog::~BuiltInConvolutionDialog()
{
  delete ui;
  if (gaussMatrix != NULL)
    delete [] gaussMatrix;
}

void BuiltInConvolutionDialog::on_okPushButton_clicked()
{
  emit confirmed(width, height, matrix, divisor, offset);
  accept();
}

void BuiltInConvolutionDialog::on_cancelPushButton_clicked()
{
  reject();
}
