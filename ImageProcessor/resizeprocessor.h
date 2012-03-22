#ifndef RESIZEPROCESSOR_H
#define RESIZEPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

#include "imagealgorithm.h"

/**
 * Class of processor to resize an image.
 */
class ResizeProcessor : public AbstractImageProcessorWithDialogOption
{
  Q_OBJECT
public:
  /**
   * Type of the size.
   */
  enum SizeType {Absolute, Relative};

  /**
   * Constructor.
   */
  ResizeProcessor();

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

private:
  /**
   * Type of the size.
   */
  SizeType _sizeType;

  /**
   * Type of the algorithm.
   */
  ImageAlgorithm::ResizeAlgorithmType _algorithmType;

  /**
   * Width.
   */
  double _width;

  /**
   * Height.
   */
  double _height;

private slots:
  /**
   * Slot used to get the confirm.
   */
  void confirm(ResizeProcessor::SizeType,
               ImageAlgorithm::ResizeAlgorithmType,
               double,
               double);
};

#endif // RESIZEPROCESSOR_H
