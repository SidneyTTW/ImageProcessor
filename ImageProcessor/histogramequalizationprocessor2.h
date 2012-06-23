#ifndef HISTOGRAMEQUALIZATIONPROCESSOR2_H
#define HISTOGRAMEQUALIZATIONPROCESSOR2_H

#include "abstractimageprocessorwithdialogoption.h"

/**
 * Class of processor to do histogram equalization.
 */
class HistogramEqualizationProcessor2 : public AbstractImageProcessorWithDialogOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  HistogramEqualizationProcessor2();

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
   * The area to do histogram equalization.
   */
  Area _area;
};
#endif // HISTOGRAMEQUALIZATIONPROCESSOR2_H
