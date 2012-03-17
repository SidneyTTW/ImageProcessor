#ifndef CONVOLUTIONPROCESSOR_H
#define CONVOLUTIONPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

#include <QVector>

/**
 * Class of processor to convert an image into black and white.
 */
class ConvolutionProcessor : public AbstractImageProcessorWithDialogOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  ConvolutionProcessor();

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
   * The matrix.
   */
  QVector<int> matrix;

  /**
   * The width of the matrix.
   */
  int matrixWidth;

  /**
   * The divisor.
   */
  int divisor;

  /**
   * The offset.
   */
  int offset;

private slots:
  /**
   * Slot used to get the confirm.
   */
  void confirm(QVector<int> matrix,
               int matrixWidth,
               int divisor,
               int offset);
};

#endif // CONVOLUTIONPROCESSOR_H
