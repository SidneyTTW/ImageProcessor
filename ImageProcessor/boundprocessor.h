#ifndef BOUNDPROCESSOR_H
#define BOUNDPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

#include <QVector>

/**
 * Class of processor to convert an image into black and white.
 */
class BoundProcessor : public AbstractImageProcessorWithDialogOption
{
  Q_OBJECT
public:
  /**
   * Flag of the bound.
   */
  enum BoundTypeFlag {UpDown = 1, DownUp = 2, LeftRight = 4, RightLeft = 8};

  /**
   * The type of the bound.
   */
  typedef QFlags<BoundTypeFlag> BoundType;

  /**
   * Constructor.
   */
  BoundProcessor();

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

  static const int UpDownConvolutionCore[9];
  static const int DownUpConvolutionCore[9];
  static const int LeftRightConvolutionCore[9];
  static const int RightLeftConvolutionCore[9];

private:
  /**
   * The type of the bound.
   */
  BoundType type;

private slots:
  /**
   * Slot used to get the confirm.
   */
  void confirm(BoundProcessor::BoundType type);
};

#endif // BOUNDPROCESSOR_H
