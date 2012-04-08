#ifndef HISTOGRAMEQUALIZATIONPROCESSOR_H
#define HISTOGRAMEQUALIZATIONPROCESSOR_H


#include "abstractimageprocessorwithsimpleoption.h"

#include "imagealgorithm.h"

class HistogramEqualizationOptionWidget;

/**
 * Class of processor to convert an image into gray one.
 */
class HistogramEqualizationProcessor : public AbstractImageProcessorWithSimpleOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  HistogramEqualizationProcessor();

  virtual MyImage::ImageTypeFlag resultType() const;
  virtual QImage *processImage(const QImage& image) const;
  virtual void processImage(QImage *image) const;
  virtual QWidget *optionWidget();
  virtual MyImage preProcessImage(const MyImage& image) const;
  virtual bool cancelWhenNewOneIsCreated() const;
  virtual void interrupt();
  virtual QString name() const;
  virtual QString toString() const;
  virtual AbstractImageProcessor *fromString(const QString& str) const;
  virtual QString description() const;
  virtual QString iconPath() const;

  /**
   * @return The tunel to change.
   */
  inline ImageAlgorithm::RGBAField getType() const
  {
    return _tunel;
  }

private:
  /**
   * The tunel to change.
   */
  ImageAlgorithm::RGBAField _tunel;

  /**
   * The option widget.
   */
  HistogramEqualizationOptionWidget *_optionWidget;

  /**
   * Calculate the map.
   *
   * @param statistic The statistic.
   * @param mapR Map of red.
   * @param mapG Map of green.
   * @param mapB Map of blue.
   * @param mapA Map of alpha.
   */
  void calculateRGBMap(const BasicStatistic& statistic,
                       int mapR[MAX_COLOR_VALUE],
                       int mapG[MAX_COLOR_VALUE],
                       int mapB[MAX_COLOR_VALUE]) const;

private slots:
  /**
   * Slot called when the option has been confirmed.
   */
  void confirm();

  /**
   * Slot used to change the tunel.
   */
  void changeToTunel(ImageAlgorithm::RGBAField tunel);
};

#endif // HISTOGRAMEQUALIZATIONPROCESSOR_H
