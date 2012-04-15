#include "myimage.h"

#include "imagealgorithm.h"

MyImage::MyImage(const QImage& image, ImageTypeFlag type) :
    _type(type)
{
  if (image.format() != QImage::Format_ARGB32)
    _image = image.convertToFormat(QImage::Format_ARGB32);
  else
    _image = image;
}

MyImage *MyImage::open(const QString& path)
{
  QImage *image = new QImage(path);
  if (image->isNull())
  {
    delete image;
    return NULL;
  }
  MyImage *result = new MyImage(image->convertToFormat(QImage::Format_ARGB32),
                                MyImage::BlackAndWhite);
  delete image;
  int width = result->_image.width();
  int height = result->_image.height();
  const unsigned char *imageDataPtr = result->_image.bits();
  int realWidth = result->_image.bytesPerLine();
  const unsigned char *backup = imageDataPtr;

  bool hasColor = false;
  bool hasGray = false;

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup + realWidth * i;
    for(int j = 0;j < width;++j)
    {
      int r, g, b, a;
      ImageAlgorithm::getRGBA(imageDataPtr, r, g, b, a);
      if (r != g || r != b)
      {
        hasColor = true;
        break;
      }
      else if (r != 0 && r != MAX_COLOR_VALUE)
        hasGray = true;
      imageDataPtr += 4;
    }
    if (hasColor)
      break;
  }
  if (hasColor)
    result->_type = Colored;
  else if (hasGray)
    result->_type = Gray;
}

void MyImage::save(const QString& path) const
{
  int width = _image.width();
  int height = _image.height();
  const unsigned char *imageDataPtr = _image.bits();
  int realWidth = _image.bytesPerLine();
  const unsigned char *backup = imageDataPtr;

  bool hasColor = false;

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup + realWidth * i;
    for(int j = 0;j < width;++j)
    {
      int r, g, b, a;
      ImageAlgorithm::getRGBA(imageDataPtr, r, g, b, a);
      if (r != g || r != b)
      {
        hasColor = true;
        break;
      }
      imageDataPtr += 4;
    }
    if (hasColor)
      break;
  }

  if (hasColor)
    _image.save(path);
  else
  {
    QImage grayImage(width, height, QImage::Format_Indexed8);
    unsigned char *grayDataPtr = grayImage.bits();
    int realWidth2 = grayImage.bytesPerLine();
    unsigned char *backup2 = grayDataPtr;
    QVector<QRgb> table;
    for (int i = 0;i < MAX_COLOR_VALUE;++i)
      table.push_back(qRgb(i, i, i));
    grayImage.setColorTable(table);

    for(int i = 0;i < height;++i)
    {
      imageDataPtr = backup + realWidth * i;
      grayDataPtr = backup2 + realWidth2 * i;
      for(int j = 0;j < width;++j)
      {
        int r, g, b, a;
        ImageAlgorithm::getRGBA(imageDataPtr, r, g, b, a);
        *grayDataPtr = (char) r;
        ++grayDataPtr;
        imageDataPtr += 4;
      }
    }
    grayImage.save(path);
  }
}
