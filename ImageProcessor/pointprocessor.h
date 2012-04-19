#ifndef POINTPROCESSOR_H
#define POINTPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

#include "myglobal.h"

/**
 * Class of processor to do point operate.
 */
class PointProcessor : public AbstractImageProcessorWithDialogOption
{
  Q_OBJECT
public:
  /**
   * Type of the threshold.
   */
  enum ProcessorType {Linear, NonLinear};

  /**
   * Constructor.
   */
  PointProcessor();

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
  static void calculateRGBMap(ProcessorType type,
                              double para1,
                              double para2,
                              int mapR[MAX_COLOR_VALUE + 1],
                              int mapG[MAX_COLOR_VALUE + 1],
                              int mapB[MAX_COLOR_VALUE + 1]);

private:
  /**
   * The type of the processor.
   */
  ProcessorType _type;

  /**
   * The first parameter.
   */
  double _para1;

  /**
   * The second parameter.
   */
  double _para2;

  /**
   * The area.
   */
  Area _area;


private slots:
  /**
   * Slot used to get the confirm.
   */
  void confirm(Area area,
               PointProcessor::ProcessorType type,
               double para1,
               double para2);
};

#endif // POINTPROCESSOR_H
