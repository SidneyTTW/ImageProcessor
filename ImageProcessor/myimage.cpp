#include "myimage.h"

MyImage::MyImage(const QImage& image, ImageTypeFlag type) :
    _type(type)
{
  if (image.format() != QImage::Format_ARGB32)
    _image = image.convertToFormat(QImage::Format_ARGB32);
  else
    _image = image;
}
