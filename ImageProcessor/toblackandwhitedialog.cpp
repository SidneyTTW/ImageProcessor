#include "toblackandwhitedialog.h"
#include "ui_toblackandwhitedialog.h"

#include "basicstatisticplot.h"
#include "imagealgorithm.h"
#include "thresholditem.h"
#include "xplotpicker.h"

ToBlackAndWhiteDialog::ToBlackAndWhiteDialog(const MyImage& image,
                                             const Area& area,
                                             QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ToBlackAndWhiteDialog),
    _area(area)
{
  ui->setupUi(this);

  _image = image.getImage();

  changing = false;
  single = true;
  black = true;
  singleThreshold = 0;

  thresholdItem = new ThresholdItem();
  plot = new BasicStatisticPlot(
      ImageAlgorithm::getStatistic(image.getImage(), ImageAlgorithm::Green));
  ui->widget_2->layout()->addWidget(plot);
  plot->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
  thresholdItem->attach(plot);
  QVector<int> thresholds;
  thresholds.push_back(0);
  thresholdItem->setThresholds(thresholds);
  connect(thresholdItem,
          SIGNAL(thresholdChanged(QVector<int>)),
          this,
          SLOT(multipleChanged(QVector<int>)));
  xPlotPicker = new XPlotPicker(QwtPlot::xBottom,
                                QwtPlot::yLeft,
                                QwtPlotPicker::VLineRubberBand,
                                QwtPicker::AlwaysOn,
                                plot->canvas());
  xPlotPicker->setRubberBandPen(QColor(0, 0, 255, 160));
  xPlotPicker->setTrackerPen(QColor(0, 0, 255, 160));
  xPlotPicker->setEnabled(true);
  connect(xPlotPicker, SIGNAL(pressAt(int)), this, SLOT(singleChanged(int)));
  connect(ui->thresholdSpinBox,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(singleChanged(int)));
  marker = new QwtPlotMarker();
  marker->setValue(0.0, 0.0);
  marker->setLineStyle(QwtPlotMarker::VLine);
  marker->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
  marker->setLinePen(QPen(Qt::green, 0, Qt::DashDotLine));
  marker->attach(plot);

  connect(ui->thresholdsEdit,
          SIGNAL(textEdited(QString)),
          this,
          SLOT(multipleTextChanged(QString)));
  connect(ui->blackButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(startWithBlack(bool)));
  connect(ui->whiteButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(startWithWhite(bool)));
  resetPreview();
}

void ToBlackAndWhiteDialog::changeToSingle()
{
  single = true;
  marker->setLineStyle(QwtPlotMarker::VLine);
  marker->setXValue(singleThreshold);
  ui->thresholdSpinBox->setValue(singleThreshold);
  plot->canvas()->removeEventFilter(thresholdItem);
  QVector<int> thresholds;
  thresholds.push_back(singleThreshold);
  thresholdItem->setThresholds(thresholds);
  if (ui->customButton->isChecked())
  {
    xPlotPicker->setEnabled(true);
    xPlotPicker->setRubberBand(QwtPicker::VLineRubberBand);
    ui->thresholdSpinBox->setEnabled(true);
  }
  plot->replot();
  resetPreview();
}

void ToBlackAndWhiteDialog::changeToMultiple()
{
  single = false;
  xPlotPicker->setEnabled(false);
  xPlotPicker->setRubberBand(QwtPicker::NoRubberBand);
  marker->setLineStyle(QwtPlotMarker::NoLine);
  plot->canvas()->installEventFilter(thresholdItem);
  QString text;
  _thresholds = thresholdItem->getThresholds();
  for (int i = 0;i < _thresholds.size();++i)
    text += tr("%1 ").arg(_thresholds[i]);
  changing = true;
  ui->thresholdsEdit->setText(text);
  changing = false;
  plot->replot();
  resetPreview();
}

void ToBlackAndWhiteDialog::singleChanged(int x)
{
  x = qBound(0, x, MAX_COLOR_VALUE);
  if (singleThreshold != x)
  {
    QVector<int> thresholds;
    thresholds.push_back(x);
    singleThreshold = x;
    thresholdItem->setThresholds(thresholds);
    marker->setXValue(x);
    if (ui->thresholdSpinBox->value() != x)
      ui->thresholdSpinBox->setValue(x);
    plot->replot();
    resetPreview();
  }
}

void ToBlackAndWhiteDialog::multipleTextChanged(QString text)
{
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
      result += text[i];
      break;
    default:
      needToChange = true;
      break;
    }
  if (needToChange)
  {
    changing = true;
    ui->thresholdsEdit->setText(result);
    changing = false;
  }
  QVector<int> thresholds;
  QStringList list = result.split(' ', QString::SkipEmptyParts);
  QString str;
  foreach (str, list)
    thresholds.push_back(str.toInt());
  _thresholds = thresholds;
  thresholdItem->setThresholds(thresholds);

  resetPreview();
}

void ToBlackAndWhiteDialog::multipleChanged(QVector<int> thresholds)
{
  _thresholds = thresholds;
  changing = true;
  QString text;
  for (int i = 0;i < thresholds.size();++i)
    text += tr("%1 ").arg(thresholds[i]);
  ui->thresholdsEdit->setText(text);
  changing = false;
  resetPreview();
}

void ToBlackAndWhiteDialog::startWithBlack(bool selected)
{
  if (!selected)
    return;
  black = true;
  thresholdItem->setStartColor(0);
  plot->replot();
  resetPreview();
}

void ToBlackAndWhiteDialog::startWithWhite(bool selected)
{
  if (!selected)
    return;
  black = false;
  thresholdItem->setStartColor(MAX_COLOR_VALUE);
  plot->replot();
  resetPreview();
}

void ToBlackAndWhiteDialog::resetPreview()
{
  QVector<int> thresholds;
  int startColor = black ? 0 : MAX_COLOR_VALUE;
  if (single)
    thresholds.append(singleThreshold);
  else
    thresholds = _thresholds;
  QImage *image = ImageAlgorithm::convertToBlackAndWhite(_image, thresholds, startColor, _area);
  ui->imageViewerWidget->setImage(*image);
  delete image;
}

ToBlackAndWhiteDialog::~ToBlackAndWhiteDialog()
{
  delete ui;
}

void ToBlackAndWhiteDialog::on_customButton_toggled(bool checked)
{
  if (!checked)
    return;
  xPlotPicker->setEnabled(true);
  xPlotPicker->setRubberBand(QwtPicker::VLineRubberBand);
  ui->thresholdSpinBox->setEnabled(true);
}

void ToBlackAndWhiteDialog::on_OTSUButton_toggled(bool checked)
{
  if (!checked)
    return;
  xPlotPicker->setEnabled(false);
  xPlotPicker->setRubberBand(QwtPicker::NoRubberBand);
  ui->thresholdSpinBox->setEnabled(false);
  singleChanged(ImageAlgorithm::OTSU(_image,
                                     ImageAlgorithm::Green,
                                     _area));
}

void ToBlackAndWhiteDialog::on_maxEntropyButton_toggled(bool checked)
{
  if (!checked)
    return;
  xPlotPicker->setEnabled(false);
  xPlotPicker->setRubberBand(QwtPicker::NoRubberBand);
  ui->thresholdSpinBox->setEnabled(false);
  singleChanged(ImageAlgorithm::maxEntropy(_image,
                                           ImageAlgorithm::Green,
                                           _area));
}

void ToBlackAndWhiteDialog::on_okPushButton_clicked()
{
  ToBlackAndWhiteProcessor::ThresholdType type =
      ToBlackAndWhiteProcessor::Custom;
  if (ui->OTSUButton->isChecked())
    type = ToBlackAndWhiteProcessor::OTSU;
  else if (ui->maxEntropyButton->isChecked())
    type = ToBlackAndWhiteProcessor::MaxEntropy;
  int startColor = black ? 0 : MAX_COLOR_VALUE;
  QVector<int> thresholds;
  if (single)
    thresholds.append(singleThreshold);
  else
    thresholds = _thresholds;
  emit confirm(type, startColor, thresholds, _area);
  accept();
}

void ToBlackAndWhiteDialog::on_cancelPushButton_clicked()
{
  reject();
}

void ToBlackAndWhiteDialog::on_tabWidget_currentChanged(int index)
{
  if (index == 0)
    changeToSingle();
  else if (index == 1)
    changeToMultiple();
}
