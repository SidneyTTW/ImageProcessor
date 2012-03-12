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
  virtual MyImage *processImage(const MyImage& image) const;
  virtual QDialog *getOptionDialog(Area area, const MyImage& image);

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
