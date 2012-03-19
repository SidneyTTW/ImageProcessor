#ifndef MYIMAGE_H
#define MYIMAGE_H

#include <QImage>

class ImageAlgorithm;

/**
 * Class to wrap QImage according to my own needs.
 * The image will be converted into QImage::Format_ARGB32
 */
class MyImage
{
  friend class ImageAlgorithm;
public:
  /**
   * The type flag of image.
   */
  enum ImageTypeFlag{Colored=1, Gray=2, BlackAndWhite=4, Remain=255, NoImage=256};

  /**
   * The type of images.
   */
  typedef QFlags<ImageTypeFlag> ImageType;

  /**
   * Construcot with the image.
   *
   * @param image The image.
   */
  MyImage(const QImage& image, ImageTypeFlag type);

  /**
   * @return Type of the image.
   */
  inline ImageTypeFlag getType() const
  {
    return _type;
  }

  /**
   * @return The image.
   */
  inline const QImage& getImage() const
  {
    return _image;
  }

  /**
   * Save the image.
   *
   * @param path The path of the file.
   */
  void save(const QString& path) const;

private:
  /**
   * The image.
   */
  QImage _image;

  /**
   * The type.
   */
  ImageTypeFlag _type;
};

#endif // MYIMAGE_H
