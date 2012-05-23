#ifndef DILATIONEROSIONPROCESSOR_H
#define DILATIONEROSIONPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

/**
 * Class of processor to do dilation/erosion/open/close operation.
 */
class DilationErosionProcessor : public AbstractImageProcessorWithDialogOption
{
  Q_OBJECT
public:
  /**
   * Type of the operation.
   */
  enum DilationErosionType{Dilation, Erosion, Open, Close};

  /**
   * Constructor.
   */
  DilationErosionProcessor();

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
   * Type of the operation.
   */
  DilationErosionType _type;

  /**
   * Width of the filter.
   */
  int _width;

  /**
   * Height of the filter.
   */
  int _height;

  /**
   * The area to filt.
   */
  Area _area;

  /**
   * Matrix used of the operation.
   */
  QVector<int> _matrix;

  /**
   * Center of the operation.
   */
  int _center;

  /**
   * Whether to use the gray filter or black and white filter.
   */
  bool _gray;

private slots:
  /**
   * Slot used to get the confirm.
   */
  void confirm(DilationErosionProcessor::DilationErosionType type,
               int width,
               int height,
               int center,
               bool gray,
               Area area,
               QVector<int> matrix);
};

#endif // DILATIONEROSIONPROCESSOR_H
