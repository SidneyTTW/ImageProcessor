#ifndef ABSTRACTIMAGEPROCESSOR_H
#define ABSTRACTIMAGEPROCESSOR_H

#include <QObject>
#include "myimage.h"

/**
 * Abstract class of image processor.
 * Be aware that a static function called confiureInstance() which returns
 * the configure instance is needed in each of the realized class.
 */
class AbstractImageProcessor : public QObject
{
  Q_OBJECT
public:
  /**
   * @return The types of input it can accept.
   */
  virtual MyImage::ImageType acceptableType() const=0;

  /**
   * @return The types of result.
   */
  virtual MyImage::ImageTypeFlag resultType() const=0;

  /**
   * @param type The type.
   *
   * @return Whether the type can be accepted.
   */
  bool inline canAccept(MyImage::ImageTypeFlag type) const
  {
    MyImage::ImageType acceptable = acceptableType();
    return acceptable.testFlag(type);
  }

  /**
   * Process image according to the given image and option.
   *
   * @param image The given image.
   * @return The processed image.
   */
  virtual MyImage *processImage(const MyImage& image) const=0;

signals:
  /**
   * Signal emitted when a new processor is created.
   */
  void processorCreated(AbstractImageProcessor *newProcessor);
};

#endif // ABSTRACTIMAGEPROCESSOR_H
