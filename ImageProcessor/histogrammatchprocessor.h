#ifndef HISTOGRAMMATCHPROCESSOR_H
#define HISTOGRAMMATCHPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

#include "basicstatistic.h"

/**
 * Class of processor to do histogram match between two images.
 */
class HistogramMatchProcessor : public AbstractImageProcessorWithDialogOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  HistogramMatchProcessor();

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

  /**
   * Calculate the map.
   *
   * @param statistic1 The statistic of original image.
   * @param statistic2 The statistic of another image.
   * @param mapR Map of red.
   * @param mapG Map of green.
   * @param mapB Map of blue.
   */
  static void calculateRGBMap(const BasicStatistic& statistic1,
                              const BasicStatistic& statistic2,
                              int mapR[MAX_COLOR_VALUE + 1],
                              int mapG[MAX_COLOR_VALUE + 1],
                              int mapB[MAX_COLOR_VALUE + 1]);

private:
  /**
   * Path of the second image.
   */
  QString _path;

private slots:
  /**
   * Slot used to get the confirm.
   */
  void confirm(QString path);
};

#endif // HISTOGRAMMATCHPROCESSOR_H
