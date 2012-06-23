#ifndef DISTANCETRANSFORMPROCESSOR_H
#define DISTANCETRANSFORMPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

#include "imagealgorithm.h"

/**
 * Class of processor to resize an image.
 */
class DistanceTransformProcessor : public AbstractImageProcessorWithDialogOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  DistanceTransformProcessor();

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
   * Type.
   */
  ImageAlgorithm::DistanceTransformType type;

  /**
   * Size.
   */
  int size;

private slots:
  /**
   * Slot used to get the confirm.
   */
  void confirm(ImageAlgorithm::DistanceTransformType type, int size);
};

#endif // DISTANCETRANSFORMPROCESSOR_H
