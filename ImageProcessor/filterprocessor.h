#ifndef FILTERPROCESSOR_H
#define FILTERPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

/**
 * Class of processor to filt an image.
 */
class FilterProcessor : public AbstractImageProcessorWithDialogOption
{
  Q_OBJECT
public:
  /**
   * Type of the filter.
   * Roberts operator, Prewitt operator, Sobel operator, Gaussian filter and
   * mean filter are all convolution.
   * May add other filters later.
   */
  enum FilterType{Midian, Convolution};

  /**
   * Constructor.
   */
  FilterProcessor();

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
   * Type of the filter.
   */
  FilterType _type;

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
   * Matrix used of the convolution filter.
   */
  QVector<int> _convolutionMatrix;

  /**
   * Convolution of the convolution.
   */
  int _convolutionDivisor;

  /**
   * Offset of the convolution.
   */
  int _convolutionOffset;

private slots:
  /**
   * Slot used to get the confirm.
   */
  void confirm(FilterProcessor::FilterType type,
               int width,
               int height,
               Area area,
               QVector<int> convolutionMatrix,
               int convolutionDivisor,
               int convolutionOffset);
};

#endif // FILTERPROCESSOR_H
