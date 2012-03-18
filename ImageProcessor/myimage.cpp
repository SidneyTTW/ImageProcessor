#include "myimage.h"

MyImage::MyImage(const QImage& image, ImageTypeFlag type) :
    _type(type)
{
  if (image.format() != QImage::Format_ARGB32)
    _image = image.convertToFormat(QImage::Format_ARGB32);
  else
    _image = image;
}

void MyImage::save(const QString& path) const
{
  switch (_type)
  {
  case MyImage::Colored:
    _image.save(path);
    break;
  case MyImage::Gray:
  case MyImage::BlackAndWhite:
    _image.convertToFormat(QImage::Format_Indexed8).save(path);
    break;
  default:
    break;
  }
}
