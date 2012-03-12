#include "tograyoptionwidget.h"
#include "ui_tograyoptionwidget.h"

ToGrayOptionWidget::ToGrayOptionWidget
    (ImageAlgorithm::ImageToGrayAlgorithmType type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToGrayOptionWidget)
{
  ui->setupUi(this);
  switch (type) {
  case ImageAlgorithm::Green:
    ui->greenButton->setChecked(true);
    break;
  case ImageAlgorithm::Float:
    ui->floatButton->setChecked(true);
    break;
  case ImageAlgorithm::Integer:
    ui->integerButton->setChecked(true);
    break;
  case ImageAlgorithm::Displacement:
    ui->displacementButton->setChecked(true);
    break;
  case ImageAlgorithm::Average:
    ui->averageButton->setChecked(true);
    break;
  }
  connect(ui->greenButton, SIGNAL(toggled(bool)), this, SLOT(buttonTroggled(bool)));
  connect(ui->averageButton, SIGNAL(toggled(bool)), this, SLOT(buttonTroggled(bool)));
  connect(ui->floatButton, SIGNAL(toggled(bool)), this, SLOT(buttonTroggled(bool)));
  connect(ui->integerButton, SIGNAL(toggled(bool)), this, SLOT(buttonTroggled(bool)));
  connect(ui->displacementButton, SIGNAL(toggled(bool)), this, SLOT(buttonTroggled(bool)));

  connect(ui->confirmButton, SIGNAL(clicked()), this, SIGNAL(confirm()));
}

void ToGrayOptionWidget::buttonTroggled(bool checked)
{
  if (!checked)
    return;
  if (ui->greenButton->isChecked())
    emit optionChanged(ImageAlgorithm::Green);
  else if (ui->floatButton->isChecked())
    emit optionChanged(ImageAlgorithm::Float);
  else if (ui->integerButton->isChecked())
    emit optionChanged(ImageAlgorithm::Integer);
  else if (ui->displacementButton->isChecked())
    emit optionChanged(ImageAlgorithm::Displacement);
  else if (ui->averageButton->isChecked())
    emit optionChanged(ImageAlgorithm::Average);
}

ToGrayOptionWidget::~ToGrayOptionWidget()
{
  delete ui;
}
