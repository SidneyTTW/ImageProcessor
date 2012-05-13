#ifndef RGBMAPPROCESSOR_H
#define RGBMAPPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

#include "myglobal.h"

/**
 * Class of processor to use RGB map.
 */
class RGBMapProcessor : public AbstractImageProcessorWithDialogOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  RGBMapProcessor();

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
   * The map of the colors.
   */
  int map[3][MAX_COLOR_VALUE + 1];

  /**
   * The area to filt.
   */
  Area _area;

private slots:
  /**
   * Slot used to get the confirm.
   */
  void confirm(int *map, Area area);
};

#endif // RGBMAPPROCESSOR_H
