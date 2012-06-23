#ifndef SKELETONPROCESSOR_H
#define SKELETONPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

/**
 * Class of processor to resize an image.
 */
class SkeletonProcessor : public AbstractImageProcessorWithDialogOption
{
public:
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
};

#endif // SKELETONPROCESSOR_H
