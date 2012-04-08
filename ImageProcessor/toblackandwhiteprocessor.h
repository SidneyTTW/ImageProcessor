#ifndef TOBLACKANDWHITEPROCESSOR_H
#define TOBLACKANDWHITEPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

#include <QVector>

/**
 * Class of processor to convert an image into black and white.
 */
class ToBlackAndWhiteProcessor : public AbstractImageProcessorWithDialogOption
{
  Q_OBJECT
public:
  /**
   * Type of the threshold.
   */
  enum ThresholdType {OTSU, MaxEntropy, Custom};

  /**
   * Constructor.
   */
  ToBlackAndWhiteProcessor();

  virtual Area::AreaType acceptableAreaType() const;
  virtual Area::AreaTypeFlag resultAreaType() const;
  virtual Area resultArea() const;
  virtual MyImage::ImageType acceptableType() const;
  virtual MyImage::ImageTypeFlag resultType() const;
  virtual QImage *processImage(const QImage& image) const;
  virtual void processImage(QImage *image) const;
  virtual QDialog *getOptionDialog(Area area, const MyImage& image);
  virtual QString name() const;
  virtual QString toString() const;
  virtual AbstractImageProcessor *fromString(const QString& str) const;
  virtual QString description() const;
  virtual QString iconPath() const;

private:
  /**
   * The thresholds.
   */
  QVector<int> thresholds;

  /**
   * The type of the convertion.
   */
  ThresholdType type;

  /**
   * The start color.
   */
  int startColor;

private slots:
  /**
   * Slot used to get the confirm.
   */
  void confirm(ToBlackAndWhiteProcessor::ThresholdType type,
               int startColor,
               QVector<int> thresholds);
};

#endif // TOBLACKANDWHITEPROCESSOR_H
