#ifndef ROTATEPROCESSOR_H
#define ROTATEPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

#include "imagealgorithm.h"

/**
 * Class of processor to rotate an image.
 */
class RotateProcessor : public AbstractImageProcessorWithDialogOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  RotateProcessor();

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
   * Type of the algorithm.
   */
  ImageAlgorithm::GeometricAlgorithmType _algorithmType;

  /**
   * Angle.
   */
  double _angle;

  /**
   * The area to filt.
   */
  Area _area;

private slots:
  /**
   * Slot used to get the confirm.
   */
  void confirm(Area area,
               ImageAlgorithm::GeometricAlgorithmType,
               double);
};

#endif // ROTATEPROCESSOR_H
