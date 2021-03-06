#include "imagealgorithm.h"

#include <QDir>
#include <qmath.h>

inline QPointF calculatePos(double r, double angle)
{
  return QPointF(r * qCos(angle), r * qSin(angle));
}

inline double calculateAngle(double x, double y)
{
  qreal result = qAtan(qAbs(y / x));
  if (x < 0 && y >= 0)
    return PI - result;
  if (x <= 0 && y < 0)
    return PI + result;
  if (x > 0 && y < 0)
    return 2 * PI - result;
  if (result != result) // NaN !!!
    return 0;
  return result;
}

/**
 * Find the range of the rotation.
 *
 * @param width The width.
 * @param height The height.
 * @param angle The angle.
 * @param left The left bound.
 * @param right The right bound.
 * @param top The top bound.
 * @param bottom The bottom bound.
 */
void rotateFindRange(int width, int height, double angle,
                     int& left, int& right, int& top, int& bottom)
{
  left = right = top = bottom = 0;
  double vr[3];
  double va[3];
  vr[0] = width;
  vr[1] = qSqrt(width * width + height * height);
  vr[2] = height;
  va[0] = angle;
  va[1] = angle + calculateAngle(width, height);
  va[2] = 3.14159 / 2 + angle;
  for (int i = 0;i < 3;++i)
  {
    QPointF v = calculatePos(vr[i], va[i]);
    if (v.x() < left)
      left = v.x();
    if (v.x() > right)
      right = v.x();
    if (v.y() < top)
      top = v.y();
    if (v.y() > bottom)
      bottom = v.y();
  }
}

// Thanks to http://blog.csdn.net/nrc_douningbo/article/details/5929106
// Although there was something wrong, my program was sped up a lot.
QImage *ImageAlgorithm::convertToGrayScale(const QImage& image,
                                           ImageToGrayAlgorithmType type)
{
  if (!validType(image))
    return NULL;
  int width = image.width();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  QImage *grayImg = new QImage(width, height, SUPPORTED_FORMAT);
  unsigned char *grayImgDataPtr = grayImg->bits();
  int realWidth1 = image.bytesPerLine();
  int realWidth2 = grayImg->bytesPerLine();
  const unsigned char *backup1 = imageDataPtr;
  unsigned char *backup2 = grayImgDataPtr;

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    grayImgDataPtr = backup2 + realWidth2 * i;
    for(int j = 0;j < width;++j)
    {
      int r, g, b, a;
      getRGBA(imageDataPtr, r, g, b, a);
      int gray = calculateGray(r, g, b, type);
      setRGBA(grayImgDataPtr, gray, gray, gray, MAX_COLOR_VALUE);
      imageDataPtr += 4;
      grayImgDataPtr += 4;
    }
  }
  return grayImg;
}

void ImageAlgorithm::convertToGrayScale(QImage *image,
                                        ImageToGrayAlgorithmType type)
{
  if (!validType(*image))
    return;
  int width = image->width();
  int height = image->height();
  unsigned char *imageDataPtr = image->bits();
  int realWidth = image->bytesPerLine();
  unsigned char *backup = imageDataPtr;

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup + realWidth * i;
    for(int j = 0;j < width;++j)
    {
      int r, g, b, a;
      getRGBA(imageDataPtr, r, g, b, a);
      int gray = calculateGray(r, g, b, type);
      setRGBA(imageDataPtr, gray, gray, gray, MAX_COLOR_VALUE);
      imageDataPtr += 4;
    }
  }
}

QImage *ImageAlgorithm::convertToBlackAndWhite(const QImage& image,
                                               QVector<int> threshold,
                                               int startColor,
                                               Area area)
{
  if (startColor != 0)
    startColor = MAX_COLOR_VALUE;
  int coloreMap[MAX_COLOR_VALUE + 1];
  int currentIndex = 0;
  int currentColor = startColor;
  for (int i = 0;i <= MAX_COLOR_VALUE;++i)
  {
    if (currentIndex < threshold.size() && threshold[currentIndex] == i)
    {
      ++currentIndex;
      currentColor = MAX_COLOR_VALUE - currentColor;
    }
    coloreMap[i] = currentColor;
  }

  if (!validType(image))
    return NULL;
  int width = image.width();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  QImage *blackAndWhiteImg = new QImage(width, height, SUPPORTED_FORMAT);
  unsigned char *blackAndWhiteImgDataPtr = blackAndWhiteImg->bits();
  int realWidth1 = image.bytesPerLine();
  int realWidth2 = blackAndWhiteImg->bytesPerLine();
  const unsigned char *backup1 = imageDataPtr;
  unsigned char *backup2 = blackAndWhiteImgDataPtr;

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    blackAndWhiteImgDataPtr = backup2 + realWidth2 * i;
    for(int j = 0;j < width;++j)
    {
      int r, g, b, a;
      getRGBA(imageDataPtr, r, g, b, a);
      if ((area.getType() == area.TypeEmpty) || area.in(j, i))
      {
        int gray = calculateGray(r, g, b, Green);
        setRGBA(blackAndWhiteImgDataPtr,
                coloreMap[gray],
                coloreMap[gray],
                coloreMap[gray],
                MAX_COLOR_VALUE);
      }
      else
        setRGBA(blackAndWhiteImgDataPtr, r, g, b, a);
      imageDataPtr += 4;
      blackAndWhiteImgDataPtr += 4;
    }
  }
  return blackAndWhiteImg;
}

void ImageAlgorithm::convertToBlackAndWhite(QImage *image,
                                            QVector<int> threshold,
                                            int startColor,
                                            Area area)
{
  if (startColor != 0)
    startColor = MAX_COLOR_VALUE;
  int coloreMap[MAX_COLOR_VALUE + 1];
  int currentIndex = 0;
  int currentColor = startColor;
  for (int i = 0;i <= MAX_COLOR_VALUE;++i)
  {
    if (currentIndex < threshold.size() && threshold[currentIndex] == i)
    {
      ++currentIndex;
      currentColor = MAX_COLOR_VALUE - currentColor;
    }
    coloreMap[i] = currentColor;
  }

  if (!validType(*image))
    return;
  int width = image->width();
  int height = image->height();
  unsigned char *imageDataPtr = image->bits();
  int realWidth = image->bytesPerLine();
  unsigned char *backup = imageDataPtr;

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup + realWidth * i;
    for(int j = 0;j < width;++j)
    {
      if (area.getType() == Area::TypeAll || area.in(j, i))
      {
        int r, g, b, a;
        getRGBA(imageDataPtr, r, g, b, a);
        int gray = calculateGray(r, g, b, Green);
        setRGBA(imageDataPtr,
                coloreMap[gray],
                coloreMap[gray],
                coloreMap[gray],
                MAX_COLOR_VALUE);
      }
      imageDataPtr += 4;
    }
  }
}

QImage *ImageAlgorithm::reverse(const QImage& image)
{
  if (!validType(image))
    return NULL;
  int width = image.width();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  QImage *reversedImg = new QImage(width, height, SUPPORTED_FORMAT);
  unsigned char *reversedImgDataPtr = reversedImg->bits();
  int realWidth1 = image.bytesPerLine();
  int realWidth2 = reversedImg->bytesPerLine();
  const unsigned char *backup1 = imageDataPtr;
  unsigned char *backup2 = reversedImgDataPtr;

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    reversedImgDataPtr = backup2 + realWidth2 * i;
    for(int j = 0;j < width;++j)
    {
      int r, g, b, a;
      getRGBA(imageDataPtr, r, g, b, a);
      setRGBA(reversedImgDataPtr,
              MAX_COLOR_VALUE - r,
              MAX_COLOR_VALUE - g,
              MAX_COLOR_VALUE - b,
              a);
      imageDataPtr += 4;
      reversedImgDataPtr += 4;
    }
  }
  return reversedImg;
}

void ImageAlgorithm::reverse(QImage *image)
{
  if (!validType(*image))
    return;
  int width = image->width();
  int height = image->height();
  unsigned char *imageDataPtr = image->bits();
  int realWidth = image->bytesPerLine();
  unsigned char *backup = imageDataPtr;

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup + realWidth * i;
    for(int j = 0;j < width;++j)
    {
      int r, g, b, a;
      getRGBA(imageDataPtr, r, g, b, a);
      setRGBA(imageDataPtr,
              MAX_COLOR_VALUE - r,
              MAX_COLOR_VALUE - g,
              MAX_COLOR_VALUE - b,
              a);
      imageDataPtr += 4;
    }
  }
}

QImage *ImageAlgorithm::changeRGBWithMap(const QImage& image,
                                         const int mapR[MAX_COLOR_VALUE + 1],
                                         const int mapG[MAX_COLOR_VALUE + 1],
                                         const int mapB[MAX_COLOR_VALUE + 1],
                                         RGBAField tunel,
                                         Area area)
{
  if (!validType(image))
    return NULL;
  int width = image.width();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  QImage *resultImg = new QImage(image);
  unsigned char *resultImgDataPtr = resultImg->bits();
  int realWidth1 = image.bytesPerLine();
  int realWidth2 = resultImg->bytesPerLine();
  const unsigned char *backup1 = imageDataPtr;
  unsigned char *backup2 = resultImgDataPtr;

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    resultImgDataPtr = backup2 + realWidth2 * i;
    for(int j = 0;j < width;++j)
    {
      int r, g, b, a;
      getRGBA(imageDataPtr, r, g, b, a);
      if ((area.getType() == area.TypeEmpty) || area.in(j, i))
      {
        if (tunel.testFlag(Field_R))
          r = mapR[r];
        if (tunel.testFlag(Field_G))
          g = mapG[g];
        if (tunel.testFlag(Field_B))
          b = mapB[b];
        setRGBA(resultImgDataPtr, r, g, b, a);
      }
      else
        setRGBA(resultImgDataPtr, r, g, b, a);
      imageDataPtr += 4;
      resultImgDataPtr += 4;
    }
  }
  return resultImg;
}

void ImageAlgorithm::changeRGBWithMap(QImage *image,
                                      const int mapR[MAX_COLOR_VALUE + 1],
                                      const int mapG[MAX_COLOR_VALUE + 1],
                                      const int mapB[MAX_COLOR_VALUE + 1],
                                      RGBAField tunel,
                                      Area area)
{
  if (!validType(*image))
    return;
  int width = image->width();
  int height = image->height();
  unsigned char *imageDataPtr = image->bits();
  int realWidth = image->bytesPerLine();
  unsigned char *backup = imageDataPtr;

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup + realWidth * i;
    for(int j = 0;j < width;++j)
    {
      int r, g, b, a;
      getRGBA(imageDataPtr, r, g, b, a);
      if ((area.getType() == area.TypeEmpty) || area.in(j, i))
      {
        if (tunel.testFlag(Field_R))
          r = mapR[r];
        if (tunel.testFlag(Field_G))
          g = mapG[g];
        if (tunel.testFlag(Field_B))
          b = mapB[b];
        setRGBA(imageDataPtr, r, g, b, a);
      }
      else
        setRGBA(imageDataPtr, r, g, b, a);
      imageDataPtr += 4;
    }
  }
}

QImage *ImageAlgorithm::resize(const QImage& image,
                               Area area,
                               int newWidth,
                               int newHeight,
                               GeometricAlgorithmType type)
{
  if (!validType(image))
    return NULL;
  int width = image.width();
  int height = image.height();
  int resultImageWidth = newWidth;
  int resultImageHeight = newHeight;
  QRect rect = area.bound();
  if (area.getType() != Area::TypeEmpty)
  {
    resultImageWidth = qMax(newWidth + rect.left(), width);
    resultImageHeight = qMax(newHeight + rect.top(), height);
  }
  const unsigned char *imageDataPtr = image.bits();
  QImage *resultImg = new QImage(resultImageWidth,
                                 resultImageHeight,
                                 SUPPORTED_FORMAT);
  unsigned char *resultImgDataPtr = resultImg->bits();
  int realWidth1 = image.bytesPerLine();
  int realWidth2 = resultImg->bytesPerLine();
  const unsigned char *backup1 = imageDataPtr;
  unsigned char *backup2 = resultImgDataPtr;

  memset(resultImgDataPtr, 0, resultImageWidth * resultImageHeight);

  // Copy the not effected part
  if (area.getType() != Area::TypeEmpty)
  {
    for (int i = 0;i < qMin(height, resultImageHeight);++i)
    {
      imageDataPtr = backup1 + i * realWidth1;
      resultImgDataPtr = backup2 + i * realWidth2;
      for (int j = 0;j < qMin(width, resultImageWidth);++j)
      {
        if (!area.in(j, i))
          memcpy(resultImgDataPtr, imageDataPtr, 4);
        imageDataPtr += 4;
        resultImgDataPtr += 4;
      }
    }
  }

  imageDataPtr = backup1;

  int xFrom = 0;
  int yFrom = 0;
  int xTo = resultImageWidth;
  int yTo = resultImageHeight;
  if (area.getType() != Area::TypeEmpty)
  {
    xFrom = rect.left();
    yFrom = rect.top();
    xTo = xFrom + newWidth;
    yTo = yFrom + newHeight;
  }
  // The resized part
  for(int i = yFrom;i < yTo;++i)
  {
    double originalY = 1.0 * i * (height - 1) / newHeight;
    if (area.getType() != Area::TypeEmpty)
      originalY = rect.top() + 1.0 * (i - rect.top()) * (rect.height() - 1) / newHeight;
    resultImgDataPtr = backup2 + realWidth2 * i + 4 * xFrom;
    for(int j = xFrom;j < xTo;++j)
    {
      double originalX = 1.0 * j * (width - 1) / newWidth;
      if (area.getType() != Area::TypeEmpty)
        originalX = rect.left() + 1.0 * (j - rect.left()) * (rect.width() - 1) / newWidth;
      int floorX = qFloor(originalX);
      int floorY = qFloor(originalY);
      if (area.getType() == Area::TypeEmpty || area.in(floorX, floorY))
      {
        int tr = 0, tg = 0, tb = 0, ta = 0;
        switch (type)
        {
        case NearestNeighbor:
          getRGBA(imageDataPtr + pixelOffset(realWidth1,
                                             qRound(originalX),
                                             qRound(originalY)),
                  tr, tg, tb, ta);
          break;
        case Bilinear:
          {
            int sr[4], sg[4], sb[4], sa[4];
            getRGBA(imageDataPtr + pixelOffset(realWidth1,
                                               floorX,
                                               floorY),
                    sr[0], sg[0], sb[0], sa[0]);
            getRGBA(imageDataPtr + pixelOffset(realWidth1,
                                               qCeil(originalX),
                                               floorY),
                    sr[1], sg[1], sb[1], sa[1]);
            getRGBA(imageDataPtr + pixelOffset(realWidth1,
                                               floorX,
                                               qCeil(originalY)),
                    sr[2], sg[2], sb[2], sa[2]);
            getRGBA(imageDataPtr + pixelOffset(realWidth1,
                                               qCeil(originalX),
                                               qCeil(originalY)),
                    sr[3], sg[3], sb[3], sa[3]);
            double factors[4];
            factors[0] = (1 - (originalX - floorX)) * (1 - (originalY - floorY));
            factors[1] = (originalX - floorX) * (1 - (originalY - floorY));
            factors[2] = (1 - (originalX - floorX)) * (originalY - floorY);
            factors[3] = (originalX - floorX) * (originalY - floorY);
            for (int k = 0;k < 4;++k)
            {
              tr += sr[k] * factors[k];
              tg += sg[k] * factors[k];
              tb += sb[k] * factors[k];
              ta += sa[k] * factors[k];
            }
            break;
          }
        case Bicubic:
          break;
        }
        setRGBA(resultImgDataPtr, tr, tg, tb, ta);
      }
      resultImgDataPtr += 4;
    }
  }
  return resultImg;
}

void ImageAlgorithm::resize(QImage *image,
                            Area area,
                            int newWidth,
                            int newHeight,
                            GeometricAlgorithmType type)
{
  QImage *result = resize(*image, area, newWidth, newHeight, type);
  if (result != NULL)
  {
    *result = *result;
    delete result;
  }
}

QImage *ImageAlgorithm::changeHSL(const QImage& image,
                                  double hOffset,
                                  double sOffset,
                                  double lOffset)
{
  if (!validType(image))
    return NULL;
  int width = image.width();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  QImage *resultImg = new QImage(width, height, SUPPORTED_FORMAT);
  unsigned char *resultImgDataPtr = resultImg->bits();
  int realWidth1 = image.bytesPerLine();
  int realWidth2 = resultImg->bytesPerLine();
  const unsigned char *backup1 = imageDataPtr;
  unsigned char *backup2 = resultImgDataPtr;

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    resultImgDataPtr = backup2 + realWidth2 * i;
    for(int j = 0;j < width;++j)
    {
      int r, g, b, a;
      getRGBA(imageDataPtr, r, g, b, a);
      double h, s, l;
      RGB2HSL(r, g, b, h, s, l);
      h += hOffset;
      while (h < 0)
        h += 1;
      while (h > 1)
        h -= 1;
      s = qBound(0.0, s + sOffset, 1.0);
      l = qBound(0.0, l + lOffset, 1.0);
      HSL2RGB(h, s, l, r, g, b);
      setRGBA(resultImgDataPtr, r, g, b, a);
      imageDataPtr += 4;
      resultImgDataPtr += 4;
    }
  }
  return resultImg;
}

void ImageAlgorithm::changeHSL(QImage *image,
                               double hOffset,
                               double sOffset,
                               double lOffset)
{
  if (!validType(*image))
    return;
  int width = image->width();
  int height = image->height();
  unsigned char *imageDataPtr = image->bits();
  int realWidth1 = image->bytesPerLine();
  unsigned char *backup1 = imageDataPtr;

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    for(int j = 0;j < width;++j)
    {
      int r, g, b, a;
      getRGBA(imageDataPtr, r, g, b, a);
      double h, s, l;
      RGB2HSL(r, g, b, h, s, l);
      h += hOffset;
      while (h < 0)
        h += 1;
      while (h > 1)
        h -= 1;
      s = qBound(0.0, s + sOffset, 1.0);
      l = qBound(0.0, l + lOffset, 1.0);
      HSL2RGB(h, s, l, r, g, b);
      setRGBA(imageDataPtr, r, g, b, a);
      imageDataPtr += 4;
    }
  }
}

QImage *ImageAlgorithm::redEyeReduction(const QImage& image, const Area& area)
{
  if (!validType(image))
    return NULL;
  int width = image.width();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  QImage *resultImg = new QImage(width, height, SUPPORTED_FORMAT);
  unsigned char *resultImgDataPtr = resultImg->bits();
  int realWidth1 = image.bytesPerLine();
  int realWidth2 = resultImg->bytesPerLine();
  const unsigned char *backup1 = imageDataPtr;
  unsigned char *backup2 = resultImgDataPtr;

  memcpy(resultImgDataPtr, imageDataPtr, image.byteCount());

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    resultImgDataPtr = backup2 + realWidth2 * i;
    for (int j = 0;j < width;++j)
    {
      if ((area.getType() == area.TypeEmpty) || area.in(j, i))
      {
        int tr, tg, tb, ta;
        getRGBA(imageDataPtr, tr, tg, tb, ta);
        float redIntensity = tg + tb != 0 ? (tr / (float)(tg + tb)) : 5;
        if (redIntensity > 0.7f)
        {
          tr = (tg + tb) / 2;
          setRGBA(resultImgDataPtr, tr, tg, tb, ta);
        }
      }
      imageDataPtr += 4;
      resultImgDataPtr += 4;
    }
  }
  return resultImg;
}

void ImageAlgorithm::redEyeReduction(QImage *image, const Area& area)
{
  if (!validType(*image))
    return;
  int width = image->width();
  int height = image->height();
  unsigned char *imageDataPtr = image->bits();
  int realWidth1 = image->bytesPerLine();
  unsigned char *backup1 = imageDataPtr;

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    for (int j = 0;j < width;++j)
    {
      if ((area.getType() == area.TypeEmpty) || area.in(j, i))
      {
        int tr, tg, tb, ta;
        getRGBA(imageDataPtr, tr, tg, tb, ta);
        float redIntensity = tg + tb != 0 ? (tr / (float)(tg + tb)) : 5;
        if (redIntensity > 0.6f)
        {
          tr = (tg + tb) / 2;
          setRGBA(imageDataPtr, tr, tg, tb, ta);
        }
      }
      imageDataPtr += 4;
    }
  }
}

QImage *ImageAlgorithm::algebraOperation(const QImage& image,
                                         const QImage& image2,
                                         AlgebraOperationType type)
{
  if ((!validType(image)) || (!validType(image2)))
    return NULL;
  int width = image.width();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  const unsigned char *imageDataPtr2 = image2.bits();
  QImage *resultImg = new QImage(image);
  unsigned char *resultImgDataPtr = resultImg->bits();
  int realWidth1 = image.bytesPerLine();
  int realWidth2 = image2.bytesPerLine();
  int realWidth3 = resultImg->bytesPerLine();
  const unsigned char *backup1 = imageDataPtr;
  const unsigned char *backup2 = imageDataPtr2;
  unsigned char *backup3 = resultImgDataPtr;
  width = qMin(width, image2.width());
  height = qMin(height, image2.height());

  for (int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    imageDataPtr2 = backup2 + realWidth2 * i;
    resultImgDataPtr = backup3 + realWidth3 * i;
    for(int j = 0;j < width;++j)
    {
      int r1, g1, b1, a1;
      int r2, g2, b2, a2;
      getRGBA(imageDataPtr, r1, g1, b1, a1);
      getRGBA(imageDataPtr2, r2, g2, b2, a2);
      switch (type)
      {
      case Add:
        r1 = qBound(0,
                    r1 * a1 / MAX_COLOR_VALUE + r2 * a2 / MAX_COLOR_VALUE,
                    MAX_COLOR_VALUE);
        g1 = qBound(0,
                    g1 * a1 / MAX_COLOR_VALUE + g2 * a2 / MAX_COLOR_VALUE,
                    MAX_COLOR_VALUE);
        b1 = qBound(0,
                    b1 * a1 / MAX_COLOR_VALUE + b2 * a2 / MAX_COLOR_VALUE,
                    MAX_COLOR_VALUE);
        break;
      case Minus:
        r1 = qBound(0,
                    r1 * a1 / MAX_COLOR_VALUE - r2 * a2 / MAX_COLOR_VALUE,
                    MAX_COLOR_VALUE);
        g1 = qBound(0,
                    g1 * a1 / MAX_COLOR_VALUE - g2 * a2 / MAX_COLOR_VALUE,
                    MAX_COLOR_VALUE);
        b1 = qBound(0,
                    b1 * a1 / MAX_COLOR_VALUE - b2 * a2 / MAX_COLOR_VALUE,
                    MAX_COLOR_VALUE);
        break;
      case Multiply:
        r1 = qBound(0, r1 * r2 / MAX_COLOR_VALUE, MAX_COLOR_VALUE);
        g1 = qBound(0, g1 * g2 / MAX_COLOR_VALUE, MAX_COLOR_VALUE);
        b1 = qBound(0, b1 * b2 / MAX_COLOR_VALUE, MAX_COLOR_VALUE);
        break;
      case Divide:
        r1 = qBound(0,
                    r2 != 0 ? r1 * MAX_COLOR_VALUE / r2 : r1,
                    MAX_COLOR_VALUE);
        g1 = qBound(0,
                    g2 != 0 ? g1 * MAX_COLOR_VALUE / g2 : g1,
                    MAX_COLOR_VALUE);
        b1 = qBound(0,
                    b2 != 0 ? b1 * MAX_COLOR_VALUE / b2 : b1,
                    MAX_COLOR_VALUE);
        break;
      default:
        break;
      }
      setRGBA(resultImgDataPtr, r1, g1, b1, a1);
      imageDataPtr += 4;
      imageDataPtr2 += 4;
      resultImgDataPtr += 4;
    }
  }
  return resultImg;
}

void ImageAlgorithm::algebraOperation(QImage *image,
                                      const QImage& image2,
                                      AlgebraOperationType type)
{
  if ((!validType(*image)) || (!validType(image2)))
    return;
  int width = image->width();
  int height = image->height();
  unsigned char *imageDataPtr = image->bits();
  const unsigned char *imageDataPtr2 = image2.bits();
  int realWidth1 = image->bytesPerLine();
  int realWidth2 = image2.bytesPerLine();
  unsigned char *backup1 = imageDataPtr;
  const unsigned char *backup2 = imageDataPtr2;
  width = qMin(width, image2.width());
  height = qMin(height, image2.height());

  for (int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    imageDataPtr2 = backup2 + realWidth2 * i;
    for(int j = 0;j < width;++j)
    {
      int r1, g1, b1, a1;
      int r2, g2, b2, a2;
      getRGBA(imageDataPtr, r1, g1, b1, a1);
      getRGBA(imageDataPtr2, r2, g2, b2, a2);
      switch (type)
      {
      case Add:
        r1 = qBound(0,
                    r1 * a1 / MAX_COLOR_VALUE + r2 * a2 / MAX_COLOR_VALUE,
                    MAX_COLOR_VALUE);
        g1 = qBound(0,
                    g1 * a1 / MAX_COLOR_VALUE + g2 * a2 / MAX_COLOR_VALUE,
                    MAX_COLOR_VALUE);
        b1 = qBound(0,
                    b1 * a1 / MAX_COLOR_VALUE + b2 * a2 / MAX_COLOR_VALUE,
                    MAX_COLOR_VALUE);
        break;
      case Minus:
        r1 = qBound(0,
                    r1 * a1 / MAX_COLOR_VALUE - r2 * a2 / MAX_COLOR_VALUE,
                    MAX_COLOR_VALUE);
        g1 = qBound(0,
                    g1 * a1 / MAX_COLOR_VALUE - g2 * a2 / MAX_COLOR_VALUE,
                    MAX_COLOR_VALUE);
        b1 = qBound(0,
                    b1 * a1 / MAX_COLOR_VALUE - b2 * a2 / MAX_COLOR_VALUE,
                    MAX_COLOR_VALUE);
        break;
      case Multiply:
        r1 = qBound(0, r1 * r2 / MAX_COLOR_VALUE, MAX_COLOR_VALUE);
        g1 = qBound(0, g1 * g2 / MAX_COLOR_VALUE, MAX_COLOR_VALUE);
        b1 = qBound(0, b1 * b2 / MAX_COLOR_VALUE, MAX_COLOR_VALUE);
        break;
      case Divide:
        r1 = qBound(0,
                    r2 != 0 ? r1 * MAX_COLOR_VALUE / r2 : r1,
                    MAX_COLOR_VALUE);
        g1 = qBound(0,
                    g2 != 0 ? g1 * MAX_COLOR_VALUE / g2 : g1,
                    MAX_COLOR_VALUE);
        b1 = qBound(0,
                    b2 != 0 ? b1 * MAX_COLOR_VALUE / b2 : b1,
                    MAX_COLOR_VALUE);
        break;
      default:
        break;
      }
      setRGBA(imageDataPtr, r1, g1, b1, MAX_COLOR_VALUE);
      imageDataPtr += 4;
      imageDataPtr2 += 4;
    }
  }
}

QImage *ImageAlgorithm::rotate(const QImage& image,
                               double angle,
                               GeometricAlgorithmType type,
                               const Area& area)
{
  if (!validType(image))
    return NULL;
  int width = image.width();
  int height = image.height();
  int left, right, top, bottom;
  QRect areaBound = area.bound();
  int resultImageWidth, resultImageHeight;
  double xOffset, yOffset;
  int xCenter, yCenter;
  int xFromIncrease = 0;
  int yFromIncrease = 0;
  if (area.getType() != Area::TypeEmpty)
  {
    left = qBound(0, areaBound.left(), width);
    right = qBound(0, areaBound.right(), width);
    top = qBound(0, areaBound.top(), height);
    bottom = qBound(0, areaBound.bottom(), height);
    areaBound = QRect(QPoint(left, top), QPoint(right, bottom));
    rotateFindRange(right - left, bottom - top,
                    angle, left, right, top, bottom);
    xCenter = areaBound.left() + areaBound.width() / 2;
    yCenter = areaBound.top() + areaBound.height() / 2;
    QPointF newCenter = calculatePos(qSqrt(xCenter * xCenter + yCenter * yCenter),
                                     calculateAngle(xCenter, yCenter) + angle);
    if (xCenter - (right - left) / 2 < 0)
    {
      xFromIncrease = xCenter - (right - left) / 2;
      xCenter -= xFromIncrease;
    }
    if (yCenter - (bottom - top) / 2 < 0)
    {
      yFromIncrease = yCenter - (bottom - top) / 2;
      yCenter -= yFromIncrease;
    }
    resultImageWidth = qMax(areaBound.left() + areaBound.width() / 2 +
                            (right - left) / 2, width) -
                       xFromIncrease;
    resultImageHeight = qMax(areaBound.top() + areaBound.height() / 2 +
                             (bottom - top) / 2, height) -
                        yFromIncrease;
    xOffset = xCenter - newCenter.x();
    yOffset = yCenter - newCenter.y();
    int newWidth = right - left;
    int newHeight = bottom - top;
    left = xCenter - newWidth / 2;
    right = xCenter + newWidth / 2;
    top = yCenter - newHeight / 2;
    bottom = yCenter + newHeight / 2;
  }
  else
  {
    rotateFindRange(width, height, angle, left, right, top, bottom);
    resultImageWidth = right - left;
    resultImageHeight = bottom - top;
    xCenter = (right - left) / 2;
    yCenter = (bottom - top) / 2;
    QPointF newCenter = calculatePos(qSqrt(width * width + height * height) / 2,
                                     calculateAngle(width, height) + angle);
    xOffset = xCenter - newCenter.x();
    yOffset = yCenter - newCenter.y();
    left = 0;
    right = resultImageWidth;
    top = 0;
    bottom = resultImageHeight;
  }

  const unsigned char *imageDataPtr = image.bits();
  QImage *resultImg = new QImage(resultImageWidth,
                                 resultImageHeight,
                                 SUPPORTED_FORMAT);
  unsigned char *resultImgDataPtr = resultImg->bits();
  int realWidth1 = image.bytesPerLine();
  int realWidth2 = resultImg->bytesPerLine();
  unsigned char *backup2 = resultImgDataPtr;

  memset(resultImgDataPtr, 0, resultImageWidth * resultImageHeight);

  // Copy the not effected part
  if (area.getType() != Area::TypeEmpty)
  {
    for (int i = 0;i < resultImageHeight;++i)
    {
      resultImgDataPtr = backup2 + i * realWidth2;
      for (int j = 0;j < resultImageWidth;++j)
      {
        int originalX = j + xFromIncrease;
        int originalY = i + yFromIncrease;
        if (originalX >= 0 && originalX < width &&
            originalY >= 0 && originalY < height &&
            (!area.in(originalX, originalY)))
          memcpy(resultImgDataPtr,
                 imageDataPtr + pixelOffset(realWidth1, originalX, originalY),
                 4);
        resultImgDataPtr += 4;
      }
    }
  }

  // The rotated part
  for(int i = top;i < bottom;++i)
  {
    resultImgDataPtr = backup2 + realWidth2 * i + 4 * left;
    for(int j = left;j < right;++j)
    {
      double newX = j - xOffset;
      double newY = i - yOffset;
      double newAngle = calculateAngle(newX, newY);
      QPointF originalPos = calculatePos(qSqrt(newX * newX + newY * newY),
                                         newAngle - angle);
      double originalX = originalPos.x();
      double originalY = originalPos.y();
      int floorX = qFloor(originalX);
      int floorY = qFloor(originalY);
      if (floorX >= 0 && floorX < width && floorY >= 0 && floorY < height &&
          (area.getType() == Area::TypeEmpty || area.in(floorX, floorY)))
      {
        int tr = 0, tg = 0, tb = 0, ta = 0;
        switch (type)
        {
        case NearestNeighbor:
          getRGBA(imageDataPtr + pixelOffset(realWidth1, floorX, floorY),
                  tr, tg, tb, ta);
          break;
        case Bilinear:
          {
            int sr[4], sg[4], sb[4], sa[4];
            getRGBA(imageDataPtr + pixelOffset(realWidth1,
                                               floorX,
                                               floorY),
                    sr[0], sg[0], sb[0], sa[0]);
            getRGBA(imageDataPtr + pixelOffset(realWidth1,
                                               qMin(qCeil(originalX), width - 1),
                                               floorY),
                    sr[1], sg[1], sb[1], sa[1]);
            getRGBA(imageDataPtr + pixelOffset(realWidth1,
                                               floorX,
                                               qMin(qCeil(originalY), height - 1)),
                    sr[2], sg[2], sb[2], sa[2]);
            getRGBA(imageDataPtr + pixelOffset(realWidth1,
                                               qMin(qCeil(originalX), width - 1),
                                               qMin(qCeil(originalY), height - 1)),
                    sr[3], sg[3], sb[3], sa[3]);
            double factors[4];
            factors[0] = (1 - (originalX - floorX)) * (1 - (originalY - floorY));
            factors[1] = (originalX - floorX) * (1 - (originalY - floorY));
            factors[2] = (1 - (originalX - floorX)) * (originalY - floorY);
            factors[3] = (originalX - floorX) * (originalY - floorY);
            for (int k = 0;k < 4;++k)
            {
              tr += sr[k] * factors[k];
              tg += sg[k] * factors[k];
              tb += sb[k] * factors[k];
              ta += sa[k] * factors[k];
            }
            break;
          }
        case Bicubic:
          break;
        }
        setRGBA(resultImgDataPtr, tr, tg, tb, ta);
      }
      resultImgDataPtr += 4;
    }
  }
  return resultImg;
}

void ImageAlgorithm::rotate(QImage *image,
                            double angle,
                            GeometricAlgorithmType type,
                            const Area& area)
{
  QImage *result = rotate(*image, angle, type, area);
  if (result != NULL)
  {
    *image = *result;
    delete result;
  }
}

QImage *ImageAlgorithm::erase(const QImage& image,
                              const Area& area,
                              const QColor& color)
{
  if (!validType(image))
    return NULL;
  int r = color.red();
  int g = color.green();
  int b = color.blue();
  int a = color.alpha();
  int width = image.width();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  QImage *resultImg = new QImage(width, height, SUPPORTED_FORMAT);
  unsigned char *resultImgDataPtr = resultImg->bits();
  int realWidth2 = resultImg->bytesPerLine();
  unsigned char *backup2 = resultImgDataPtr;
  QRect bound = area.getType() == area.TypeEmpty ?
                QRect(0, 0, width, height) :
                area.bound();

  memcpy(resultImgDataPtr, imageDataPtr, image.byteCount());

  for(int i = qMax(0, bound.top());i < qMin(height, bound.bottom());++i)
  {
    resultImgDataPtr = backup2 + realWidth2 * i + 4 * qMax(0, bound.left());
    for (int j = qMax(0, bound.left());j < qMin(width, bound.right());++j)
    {
      if ((area.getType() == area.TypeEmpty) || area.in(j, i))
        setRGBA(resultImgDataPtr, r, g, b, a);
      resultImgDataPtr += 4;
    }
  }
  return resultImg;
}

void ImageAlgorithm::erase(QImage *image, const Area& area, const QColor& color)
{
  if (!validType(*image))
    return;
  int r = color.red();
  int g = color.green();
  int b = color.blue();
  int a = color.alpha();
  int width = image->width();
  int height = image->height();
  unsigned char *imageDataPtr = image->bits();
  int realWidth1 = image->bytesPerLine();
  unsigned char *backup1 = imageDataPtr;
  QRect bound = area.getType() == area.TypeEmpty ?
                QRect(0, 0, width, height) :
                area.bound();

  for(int i = qMax(0, bound.top());i < qMin(height, bound.bottom());++i)
  {
    imageDataPtr = backup1 + realWidth1 * i + 4 * qMax(0, bound.left());
    for (int j = qMax(0, bound.left());j < qMin(width, bound.right());++j)
    {
      if (area.in(j, i))
        setRGBA(imageDataPtr, r, g, b, a);
      imageDataPtr += 4;
    }
  }
}

int *getSquareKernel(int size)
{
  int *result = new int[(size * 2 + 1) * ((size * 2 + 1))];
  int count = 0;
  for (int i = - size;i <= size;i++)
    for (int j = -size;j <= size;j++)
      result[count++] = MAX_COLOR_VALUE;
  return result;
}

int *getCrossKernel(int size)
{
  int *result = new int[(size * 2 + 1) * ((size * 2 + 1))];
  int count = 0;
  for (int i = - size;i <= size;i++)
    for (int j = -size;j <= size;j++)
      result[count++] = (i == 0 || j == 0) ?
                        MAX_COLOR_VALUE :
                        -(MAX_COLOR_VALUE + 1);
  return result;
}

int *getDiscKernel(int size)
{
  int *result = new int[(size * 2 + 1) * ((size * 2 + 1))];
  int count = 0;
  for (int i = -size;i <= size;i++)
    for (int j = -size;j <= size;j++)
      result[count++] = (i * i + j * j <= size * size) ?
                        MAX_COLOR_VALUE :
                        -(MAX_COLOR_VALUE + 1);
  return result;
}

QImage* ImageAlgorithm::distanceTransform(const QImage &image,
                                          DistanceTransformType type,
                                          int size)
{
  if (!validType(image))
    return NULL;
  int width = image.width();
  int height = image.height();

  int *kernel;
  switch (type)
  {
  case BlockDistance:
    kernel = getSquareKernel(size);
    break;
  case ChessboardDistance:
    kernel = getCrossKernel(size);
    break;
  case GeometricDistance:
    kernel = getDiscKernel(size);
    break;
  default:
    return NULL;
  }

  ErosionFilter *filter = new ErosionFilter(kernel,
                                            (size * 2 + 1) * (size * 2 + 1) / 2,
                                            size * 2 + 1,
                                            size * 2 + 1);
  delete [] kernel;

  int *distance = new int[width * height];
  memset(distance, 0, sizeof(int) * width * height);

  QImage *lastImage = new QImage(image);
  int count = 0;
  while(true)
  {
    ++count;
    QImage *newImage = ImageAlgorithm::filtImage<ImageAlgorithm::ErosionFilter>(*lastImage, Area(), filter);
    bool flag = true;
    for (int j = 0;j < height;j++)
      for (int i = 0;i < width;i++)
        if (qGreen(newImage->pixel(i, j)) == 0 &&
            qGreen(lastImage->pixel(i, j)) > 0)
        {
          distance[j * width + i] = count;
          flag = false;
        }
    QImage *p = lastImage;
    lastImage = newImage;
    delete(p);
    if (flag)
      break;
  }
  QImage *resultImage = new QImage(width, height, SUPPORTED_FORMAT);
  resultImage->fill(0);
  for (int j = 0;j < height;++j)
      for (int i = 0;i < width;++i)
      {
        int value = distance[j * width + i] * MAX_COLOR_VALUE / count;
        resultImage->setPixel(i, j, qRgb(value, value, value));
      }

  delete [] distance;
  return resultImage;
}

void ImageAlgorithm::distanceTransform(QImage *image,
                                       DistanceTransformType type,
                                       int size)
{
  QImage *result = distanceTransform(*image, type, size);
  if (result != NULL)
  {
    *image = *result;
    delete result;
  }
}

bool allBlack(const QImage& image)
{
  int width = image.width();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  int realWidth = image.bytesPerLine();
  const unsigned char *backup = imageDataPtr;

  for (int i = 0;i < height;++i)
  {
    imageDataPtr = backup + realWidth * i;
    for (int j = 0;j < width;++j)
    {

      int r, g, b, a;
      ImageAlgorithm::getRGBA(imageDataPtr, r, g, b, a);
      if (r != 0 || g != 0 || b != 0)
        return false;
      imageDataPtr += 4;
    }
  }
  return true;
}

QImage *ImageAlgorithm::skeleton(const QImage &image)
{
  if (!validType(image))
    return NULL;
  const QImage *currentImage = &image;
  QVector<const QImage *> s;
  int *kernel = getCrossKernel(1);
  ErosionFilter *erosionFilter = new ErosionFilter(kernel, 4, 3, 3);
  DilationFilter *dilationFilter = new DilationFilter(kernel, 4, 3, 3);
  delete [] kernel;
  QImage *result = new QImage(image.width(), image.height(), SUPPORTED_FORMAT);
  result->fill(0);
  while (true)
  {
    if (allBlack(*currentImage))
      break;
    currentImage = filtImage<ErosionFilter>(*currentImage, Area(), erosionFilter);
    s.push_back(currentImage);
  }

  QString pathBase = QDir::homePath() + "/.ImageProcessor/";
  QDir dir = QDir(pathBase);
  if(!dir.exists())
  {
    if(!dir.mkdir(pathBase))
      return NULL;
  }

  // Make the path if it hasn't be created
  QFileInfoList fileInfoList = dir.entryInfoList();
  foreach(QFileInfo fileInfo, fileInfoList)
  {
    if(fileInfo.isFile())
    {
      QFile tmpFile(fileInfo.fileName());
      dir.remove(tmpFile.fileName());
    }
  }

  int count = 0;
  foreach (currentImage, s)
  {
    QImage *opened = filtImage<ErosionFilter>(*currentImage, Area(), erosionFilter);
    filtImage<DilationFilter>(opened, Area(), dilationFilter);
    QImage *tmpSkeleton = algebraOperation(*currentImage, *opened, Minus);
    tmpSkeleton->save(QObject::tr("%1%2.bmp").arg(pathBase).arg(count));
    ++count;
    algebraOperation(result, *tmpSkeleton, Add);
    delete tmpSkeleton;
    delete opened;
    delete currentImage;
  }

  delete erosionFilter;
  delete dilationFilter;
  return result;
}


void ImageAlgorithm::skeleton(QImage *image)
{
  QImage *result = skeleton(*image);
  if (result != NULL)
  {
    *image = *result;
    delete result;
  }
}

QImage *ImageAlgorithm::skeletonReconstruct(const QString& dirPath)
{
  QImage *result = NULL;
  QImage *mask = NULL;

  QDir dir = QDir(dirPath);
  if(!dir.exists())
    return NULL;

  int count = 0;

  QFileInfoList fileInfoList = dir.entryInfoList();
  foreach(QFileInfo fileInfo, fileInfoList)
  {
    if(fileInfo.isFile())
    {
      if (fileInfo.baseName() != "mask")
        ++count;
      else
        mask = new QImage(QImage(fileInfo.absoluteFilePath()).
                          convertToFormat(SUPPORTED_FORMAT));
    }
  }

  int *kernel = getCrossKernel(1);
  DilationFilter *filter = new DilationFilter(kernel, 4, 3, 3);
  delete [] kernel;

  for (int i = 1;i < count;++i)
  {
    QImage *image = new QImage(QImage(QObject::tr("%1/%2.bmp").arg(dirPath).arg(i)).
                               convertToFormat(SUPPORTED_FORMAT));
    if (image->isNull())
    {
      delete image;
      continue;
    }

    if (mask != NULL)
      algebraOperation(image, *mask, Multiply);

    for (int j = 0;j < i;++j)
      filtImage<DilationFilter>(image, Area(), filter);

    if (result == NULL)
    {
      result = image;
    }
    else
    {
      algebraOperation(result, *image, Add);
      delete image;
    }
  }

  if (mask != NULL)
    delete mask;
  delete filter;
  return result;
}

QImage *ImageAlgorithm::magicErase(const QImage& image,
                                   const QPoint& pos,
                                   int allowedDifference)
{
  if (!validType(image))
    return NULL;
  QImage *resultImg = new QImage(image);
  magicErase(resultImg, pos, allowedDifference);
  return resultImg;
}

void ImageAlgorithm::magicErase(QImage *image,
                                const QPoint& pos,
                                int allowedDifference)
{
  if (!validType(*image))
    return;
  int width = image->width();
  int height = image->height();
  if (pos.x() < 0 || pos.x() >= width || pos.y() < 0 || pos.y() >= height)
    return;

  bool *visited = new bool[width * height];
  int index = 0;
  int *vector = new int[width * height * 2 * 4];
  vector[0] = pos.x();
  vector[1] = pos.y();
  unsigned char *imageDataPtr = image->bits();
  int realWidth = image->bytesPerLine();

  int r, g, b, a;
  getRGBA(imageDataPtr + pixelOffset(realWidth, pos.x(), pos.y()), r, g, b, a);

  memset(visited, 0, sizeof(bool) * width * height);

  while (index >= 0)
  {
    int x = vector[index * 2], y = vector[index * 2 + 1];
    int sr, sg, sb, sa;
    int offset = pixelOffset(realWidth, x, y);
    getRGBA(imageDataPtr + offset, sr, sg, sb, sa);
    bool allowed = qAbs(sr - r) + qAbs(sg - g) + qAbs(sb - b) <= allowedDifference;
    --index;
    if (allowed)
    {
      visited[y * width + x] = true;
      setRGBA(imageDataPtr + offset, 0, 0, 0, 0);
      int xFrom = qBound(0, x - 1, width - 1);
      int xTo = qBound(0, x + 1, width - 1);
      int yFrom = qBound(0, y - 1, height - 1);
      int yTo = qBound(0, y + 1, height - 1);
      for (int i = xFrom;i <= xTo;++i)
      {
        for (int j = yFrom;j <= yTo;++j)
        {
          if ((i == x && j == y) || visited[j * width + i])
            continue;
          ++index;
          vector[index * 2] = i;
          vector[index * 2 + 1] = j;
        }
      }
    }
  }
  delete [] vector;
  delete [] visited;
}

BasicStatistic ImageAlgorithm::getStatistic(const QImage& image,
                                            ImageToGrayAlgorithmType type,
                                            const Area& area)
{
  BasicStatistic result;
  if (!validType(image))
    return result;
  int width = image.width();
  int realWidth = image.bytesPerLine();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  const unsigned char *backup = imageDataPtr;
  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup + realWidth * i;
    for(int j = 0;j < width;++j)
    {
      if (area.getType() == Area::TypeEmpty || area.in(j, i))
      {
        int r, g, b, a;
        getRGBA(imageDataPtr, r, g, b, a);
        int gray = calculateGray(r, g, b, type);
        ++result.counts[BasicStatistic::Red][r];
        ++result.counts[BasicStatistic::Green][g];
        ++result.counts[BasicStatistic::Blue][b];
        ++result.counts[BasicStatistic::Gray][gray];
        ++result._pixels;
      }
      imageDataPtr += 4;
    }
  }
  return result;
}

int ImageAlgorithm::OTSU(const QImage& image,
                         ImageToGrayAlgorithmType type,
                         Area area)
{
  BasicStatistic statistic = getStatistic(image, type, area);
  double maxDelta = 0;
  int result = 0;
  for(int i = 0;i <= MAX_COLOR_VALUE;++i)
  {
    double foreSum = 0;
    double foreWeightedSum = 0;
    double backSum = 0;
    double backWeightedSum = 0;
    double uFore = 0;
    double uBack = 0;
    double u = 0;
    for(int j = 0;j <= MAX_COLOR_VALUE;++j)
    {
      if (j <= i)
      {
        // Back
        backSum += statistic.counts[BasicStatistic::Gray][j];
        backWeightedSum += j * statistic.counts[BasicStatistic::Gray][j];
      }
      else
      {
        // Fore
        foreSum += statistic.counts[BasicStatistic::Gray][j];
        foreWeightedSum += j * statistic.counts[BasicStatistic::Gray][j];
      }
    }
    if (backSum > 0)
      uBack = backWeightedSum / backSum;
    if (foreSum > 0)
      uFore = foreWeightedSum / foreSum;
    u = backWeightedSum + foreWeightedSum;
    double tmpDelta = backSum * (u - uBack) * (u - uBack) +
                      foreSum * (u - uFore) * (u - uFore);
    if (tmpDelta > maxDelta)
    {
      maxDelta = tmpDelta;
      result = i;
    }
  }
  return result;
}

enum EntropyState {Back, Fore};

double caculateCurrentEntropy(int histogram[],int curThreshold, EntropyState state)
{
  int start, end;
  switch (state)
  {
  case Back:
    start = 0;
    end = curThreshold;
    break;
  case Fore:
    start = curThreshold;
    end = MAX_COLOR_VALUE + 1;
    break;
  }
  int count = 0;
  for(int i = start;i < end;++i)
    count += histogram[i];

  double curEntropy = 0.0;
  for(int i = start;i < end;++i)
  {
    if(histogram[i] == 0)
        continue;
    double percentage = 1.0 * histogram[i] / count;
    curEntropy += -percentage * qLn(percentage);
  }
  return curEntropy;
}

int ImageAlgorithm::maxEntropy(const QImage& image,
                               ImageToGrayAlgorithmType type,
                               Area area)
{
  BasicStatistic statistic = getStatistic(image, type, area);
  int result = 0;
  double maxEntropy = -1;
  for(int i = 0;i <= MAX_COLOR_VALUE;++i)
  {
    double curEntropy = caculateCurrentEntropy(statistic.counts[BasicStatistic::Gray], i, Fore) +
                        caculateCurrentEntropy(statistic.counts[BasicStatistic::Gray], i, Back);
    if(curEntropy > maxEntropy)
    {
        maxEntropy = curEntropy;
        result = i;
    }
  }
  return result;
}

// Thanks to http://blog.sina.com.cn/s/blog_4b700c4c0102e02s.html
// And I reserved the comments.
void ImageAlgorithm::gaussCore(int k, float d,int *result)
{
  float *ftmp = new float[(2 * k + 1) * (2 * k + 1)];
  for(int i = 0;i < k + 1;++i)               //取模板大小(2k+1) (2k+1)的右下角部分
  {
    for(int j = 0;j < k + 1;++j)
    {
      float tmp = (i * i + j * j) / d;        // d表示平滑尺度gauss模板
      ftmp[i * (k + 1) + j] = (float)(1.0 / qExp(tmp / 2));
    }
  }
  int c = qCeil(1 / ftmp[(k + 1) * (k + 1) - 1] + 0.5);     //计算归一化系数
  for(int i = 0;i < k + 1;++i)                             //给模板右下角付值
    for(int j = 0;j < k + 1;++j)
      result[(k + i) * (2 * k + 1) + (k + j)] =
          (int)(ftmp[i * (k + 1) + j] * c + 0.5);
  for(int i = k;i < 2 * k + 1;++i)                         //给模板左下角付值
    for(int j = 0;j < k + 1;++j)
      result[i * (2 * k + 1) + j] = result[i * (2 * k + 1) + (2 * k - j)];
  for(int i = 0;i < k;++i)                                 //给模板上半部分付值
    for(int j = 0;j < 2 * k + 1;++j)
      result[i * (2 * k + 1) + j] = result[(2 * k - i) * (2 * k + 1) + j];
  delete [] ftmp;
}
