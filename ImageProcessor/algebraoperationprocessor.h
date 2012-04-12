#ifndef ALGEBRAOPERATIONPROCESSOR_H
#define ALGEBRAOPERATIONPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

#include "imagealgorithm.h"

/**
 * Class of processor to do algebra operation on two images.
 */
class AlgebraOperationProcessor : public AbstractImageProcessorWithDialogOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  AlgebraOperationProcessor();

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
  ImageAlgorithm::AlgebraOperationType _type;

  /**
   * Path of the second image.
   */
  QString _path;

private slots:
  /**
   * Slot used to get the confirm.
   */
  void confirm(ImageAlgorithm::AlgebraOperationType type,
               QString path);
};

#endif // ALGEBRAOPERATIONPROCESSOR_H
