#include "basicstatisticwidget.h"
#include "ui_basicstatisticwidget.h"

#include "basicstatisticplot.h"
#include "imagealgorithm.h"
#include "imageviewwidget.h"

BasicStatisticWidget::BasicStatisticWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::BasicStatisticWidget)
{
  ui->setupUi(this);
  plot = new BasicStatisticPlot(BasicStatistic());
  zoomer = new QwtPlotZoomer(plot->canvas());
  zoomer->setZoomBase();

  ui->dockWidgetContents->layout()->addWidget(plot);

  connect(ui->greenButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(updateStatistics(bool)));
  connect(ui->floatButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(updateStatistics(bool)));
  connect(ui->integerButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(updateStatistics(bool)));
  connect(ui->displacementButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(updateStatistics(bool)));
  connect(ui->averageButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(updateStatistics(bool)));
}

void BasicStatisticWidget::setBoundedImageView(ImageViewWidget *widget)
{
  connect(widget,
          SIGNAL(imageChanged(QImage)),
          this,
          SLOT(changeImage(QImage)));
}

void BasicStatisticWidget::changeImage(const QImage& image)
{
  _image = image;
  updateStatistics(true);
}

void BasicStatisticWidget::updateStatistics(bool)
{
  ImageAlgorithm::ImageToGrayAlgorithmType type = ImageAlgorithm::Green;
  if (ui->greenButton->isChecked())
    type = ImageAlgorithm::Green;
  else if (ui->floatButton->isChecked())
    type = ImageAlgorithm::Float;
  else if (ui->integerButton->isChecked())
    type = ImageAlgorithm::Integer;
  else if (ui->displacementButton->isChecked())
    type = ImageAlgorithm::Displacement;
  else if (ui->averageButton->isChecked())
    type = ImageAlgorithm::Average;
  BasicStatistic statistic = ImageAlgorithm::getStatistic(_image, type);
  plot->setStatistic(statistic);
  zoomer->setZoomBase();
}

BasicStatisticWidget::~BasicStatisticWidget()
{
  delete ui;
  delete zoomer;
  delete plot;
}
