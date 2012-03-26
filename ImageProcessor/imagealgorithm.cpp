#include "imagealgorithm.h"

#include <qmath.h>

ImageAlgorithm::ImageAlgorithm()
{
  ImageAlgorithm::filtImage<MedianFilter>(QImage(), NULL);
  ImageAlgorithm::filtImage<ConvolutionFilter>(QImage(), NULL);
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
                                               int startColor)
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
      int gray = calculateGray(r, g, b, Green);
      setRGBA(blackAndWhiteImgDataPtr,
              coloreMap[gray],
              coloreMap[gray],
              coloreMap[gray],
              MAX_COLOR_VALUE);
      imageDataPtr += 4;
      blackAndWhiteImgDataPtr += 4;
    }
  }
  return blackAndWhiteImg;
}

void ImageAlgorithm::convertToBlackAndWhite(QImage *image,
                                            QVector<int> threshold,
                                            int startColor)
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
      int r, g, b, a;
      getRGBA(imageDataPtr, r, g, b, a);
      int gray = calculateGray(r, g, b, Green);
      setRGBA(imageDataPtr,
              coloreMap[gray],
              coloreMap[gray],
              coloreMap[gray],
              MAX_COLOR_VALUE);
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
                                         int mapR[MAX_COLOR_VALUE],
                                         int mapG[MAX_COLOR_VALUE],
                                         int mapB[MAX_COLOR_VALUE],
                                         RGBAField tunel)
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
      if (tunel.testFlag(Field_R))
        r = mapR[r];
      if (tunel.testFlag(Field_G))
        g = mapG[g];
      if (tunel.testFlag(Field_B))
        b = mapB[b];
      setRGBA(resultImgDataPtr, r, g, b, a);
      imageDataPtr += 4;
      resultImgDataPtr += 4;
    }
  }
  return resultImg;
}

void  ImageAlgorithm::changeRGBWithMap(QImage *image,
                                       int mapR[MAX_COLOR_VALUE],
                                       int mapG[MAX_COLOR_VALUE],
                                       int mapB[MAX_COLOR_VALUE],
                                       RGBAField tunel)
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
      if (tunel.testFlag(Field_R))
        r = mapR[r];
      if (tunel.testFlag(Field_G))
        g = mapG[g];
      if (tunel.testFlag(Field_B))
        b = mapB[b];
      setRGBA(imageDataPtr, r, g, b, a);
      imageDataPtr += 4;
    }
  }
}

QImage *ImageAlgorithm::resize(const QImage& image,
                               int newWidth,
                               int newHeight,
                               ResizeAlgorithmType type)
{
  if (!validType(image))
    return NULL;
  int width = image.width();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  QImage *resultImg = new QImage(newWidth, newHeight, SUPPORTED_FORMAT);
  unsigned char *resultImgDataPtr = resultImg->bits();
  int realWidth1 = image.bytesPerLine();
  int realWidth2 = resultImg->bytesPerLine();
  unsigned char *backup2 = resultImgDataPtr;

  for(int i = 0;i < newHeight;++i)
  {
    double originalY = 1.0 * i * (height - 1) / newHeight;
    resultImgDataPtr = backup2 + realWidth2 * i;
    for(int j = 0;j < newWidth;++j)
    {
      double originalX = 1.0 * j * (width - 1) / newWidth;
      int floorX = qFloor(originalX);
      int floorY = qFloor(originalY);
      int tr = 0, tg = 0, tb = 0, ta = 0;
      switch (type)
      {
      case NearestNeighbor:
        getRGBA(imageDataPtr + pixelOffset(realWidth1, originalX, originalY),
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
      resultImgDataPtr += 4;
    }
  }
  return resultImg;
}

void ImageAlgorithm::resize(QImage *image,
                            int newWidth,
                            int newHeight,
                            ResizeAlgorithmType type)
{
  QImage *result = resize(*image, newWidth, newHeight, type);
  if (result != NULL)
  {
    memcpy(image->bits(), result->bits(), result->byteCount());
    delete result;
  }
}

BasicStatistic ImageAlgorithm::getStatistic(const QImage& image,
                                            ImageToGrayAlgorithmType type)
{
  BasicStatistic result(image.width(), image.height());
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
      int r, g, b, a;
      getRGBA(imageDataPtr, r, g, b, a);
      int gray = calculateGray(r, g, b, type);
      ++result.counts[BasicStatistic::Red][r];
      ++result.counts[BasicStatistic::Green][g];
      ++result.counts[BasicStatistic::Blue][b];
      ++result.counts[BasicStatistic::Gray][gray];
      imageDataPtr += 4;
    }
  }
  return result;
}

int ImageAlgorithm::OTSU(const QImage& image,
                         ImageToGrayAlgorithmType type)
{
  BasicStatistic statistic = getStatistic(image, type);
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
                               ImageToGrayAlgorithmType type)
{
  BasicStatistic statistic = getStatistic(image, type);
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
