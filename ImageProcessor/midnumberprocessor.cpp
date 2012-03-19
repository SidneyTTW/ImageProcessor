#include "midnumberprocessor.h"

#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include "imagealgorithm.h"

MidNumberProcessor::MidNumberProcessor():
    _optionWidget(NULL),
    _width(3),
    _height(3)
{
}

MyImage::ImageTypeFlag MidNumberProcessor::resultType() const
{
  return (MyImage::ImageTypeFlag)MyImage::Remain;
}

QImage *MidNumberProcessor::processImage(const QImage& image) const
{
  QImage *result = ImageAlgorithm::midNumber(image, _width, _height);
  return result;
}

void MidNumberProcessor::processImage(QImage *image) const
{
  ImageAlgorithm::midNumber(image, _width, _height);
}

QWidget *MidNumberProcessor::optionWidget()
{
  if (_optionWidget == NULL)
  {
    _optionWidget = new QWidget();
    _optionWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    QVBoxLayout *layout = new QVBoxLayout();
    QSpinBox *widthSpinBox = new QSpinBox();
    widthSpinBox->setMinimum(1);
    widthSpinBox->setMaximum(100);
    widthSpinBox->setValue(_width);
    connect(widthSpinBox,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(changeWidth(int)));
    layout->addWidget(widthSpinBox);
    QSpinBox *heightSpinBox = new QSpinBox();
    heightSpinBox->setMinimum(1);
    heightSpinBox->setMaximum(100);
    heightSpinBox->setValue(_height);
    connect(heightSpinBox,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(changeWidth(int)));
    layout->addWidget(heightSpinBox);
    QPushButton *button = new QPushButton("Confirm");
    connect(button, SIGNAL(clicked()), this, SLOT(confirm()));
    layout->addWidget(button);
    _optionWidget->setLayout(layout);
  }
  return _optionWidget;
}

MyImage MidNumberProcessor::preProcessImage(const MyImage& image) const
{
  QImage *resultImage = ImageAlgorithm::midNumber(image.getImage(),
                                                  _width,
                                                  _height);
  if (resultImage != NULL)
  {
    MyImage result(*resultImage, image.getType());
    delete resultImage;
    return result;
  }
  else
    return image;
}

bool MidNumberProcessor::cancelWhenNewOneIsCreated() const
{
  return true;
}

void MidNumberProcessor::interrupt()
{
}

QString MidNumberProcessor::name() const
{
  return "MidNumber";
}

QString MidNumberProcessor::toString() const
{
  return tr("%1 %2").arg(_width).arg(_height);
}

AbstractImageProcessor *MidNumberProcessor::fromString(const QString& str) const
{
  QStringList list = str.split(' ', QString::SkipEmptyParts);
  if (list.size() != 2)
    return NULL;
  MidNumberProcessor *result = new MidNumberProcessor();
  result->_width = list.takeFirst().toInt();
  result->_height = list.takeFirst().toInt();
  return result;
}

void MidNumberProcessor::confirm()
{
  MidNumberProcessor *newProcessor = new MidNumberProcessor();
  newProcessor->_width = _width;
  newProcessor->_height = _height;
  emit processorCreated(newProcessor);
}

void MidNumberProcessor::changeWidth(int width)
{
  _width = width;
  emit optionChanged(this);
}

void MidNumberProcessor::changeHeight(int height)
{
  _height = height;
  emit optionChanged(this);
}
