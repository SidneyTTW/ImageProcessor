#include "imagealgorithm.h"

#include <qmath.h>

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

QImage *ImageAlgorithm::convolution(const QImage& image,
                                    const QVector<int>& matrix,
                                    int matrixWidth,
                                    int divisor,
                                    int offset)
{
  if (!validType(image))
    return NULL;
  if (matrixWidth % 2 != 1 ||
      matrix.size() % matrixWidth != 0 ||
      (matrix.size() / matrixWidth) % 2 != 1)
    return NULL;
  int matrixHeight = matrix.size() / matrixWidth;
  int borderWidth = matrixWidth / 2;
  int borderHeight = matrixHeight / 2;
  int width = image.width();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  QImage *convolutionImg = new QImage(width, height, SUPPORTED_FORMAT);
  unsigned char *convolutionImgDataPtr = convolutionImg->bits();
  int realWidth1 = image.bytesPerLine();
  int realWidth2 = convolutionImg->bytesPerLine();
  const unsigned char *backup1 = imageDataPtr;
  unsigned char *backup2 = convolutionImgDataPtr;

  const int *factors = matrix.data();
  int *offsets = new int[matrix.size()];
  for (int i = 0;i < matrixWidth;++i)
    for (int j = 0;j < matrixHeight;++j)
      offsets[i + matrixWidth * j] = 4 * (i - borderWidth) +
                                     realWidth1 * (j - borderHeight);

  memcpy(convolutionImgDataPtr, imageDataPtr, image.byteCount());

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    convolutionImgDataPtr = backup2 + realWidth2 * i;
    for (int j = 0;j < width;++j)
    {
      int sr, sg, sb, sa;
      int tr = 0, tg = 0, tb = 0;
      for (int k = 0;k < matrixHeight;++k)
      {
        for (int l = 0;l < matrixWidth;++l)
        {
          if (i + k - borderHeight < 0 ||
              i + k - borderHeight >= height ||
              j + l - borderWidth < 0 ||
              j + l - borderWidth >= width)
            continue;
          int index = k * matrixWidth + l;
          getRGBA(imageDataPtr + offsets[index], sr, sg, sb, sa);
          tr += factors[index] * sr;
          tg += factors[index] * sg;
          tb += factors[index] * sb;
        }
      }
      tr = qBound(0, tr / divisor + offset, MAX_COLOR_VALUE);
      tg = qBound(0, tg / divisor + offset, MAX_COLOR_VALUE);
      tb = qBound(0, tb / divisor + offset, MAX_COLOR_VALUE);
      setRGBA(convolutionImgDataPtr, tr, tg, tb, MAX_COLOR_VALUE);
      imageDataPtr += 4;
      convolutionImgDataPtr += 4;
    }
  }
  return convolutionImg;
}

void ImageAlgorithm::convolution(QImage *image,
                                 const QVector<int>& matrix,
                                 int matrixWidth,
                                 int divisor,
                                 int offset)
{
  QImage *result = convolution(*image, matrix, matrixWidth, divisor, offset);
  if (result != NULL)
  {
    memcpy(image->bits(), result->bits(), result->byteCount());
    delete result;
  }
}

/**
 * Find the mid number in the array.
 *
 * @param array The array.
 * @param count number of elements in the array.
 * @return The mid number.
 * @warning Will CHANGE the original array!
 */
inline int calculateMidNumber(int *array, int count)
{
  for (int i = 0;i < count / 2 + 1;++i)
  {
    for (int j = count - 1;j > i;--j)
    if (array[j] > array[j - 1])
    {
      int tmp = array[j];
      array[j] = array[i];
      array[i] = tmp;
    }
  }
  return array[count / 2];
}

QImage *ImageAlgorithm::midNumber(const QImage& image,
                                  int matrixWidth,
                                  int matrixHeight)
{
  if (!validType(image))
    return NULL;
  if (matrixWidth % 2 != 1 || matrixHeight % 2 != 1)
    return NULL;
  int borderWidth = matrixWidth / 2;
  int borderHeight = matrixHeight / 2;
  int width = image.width();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  QImage *resultImg = new QImage(width, height, SUPPORTED_FORMAT);
  unsigned char *resultImgDataPtr = resultImg->bits();
  int realWidth1 = image.bytesPerLine();
  int realWidth2 = resultImg->bytesPerLine();
  const unsigned char *backup1 = imageDataPtr;
  unsigned char *backup2 = resultImgDataPtr;

  int *valuesR = new int[matrixWidth * matrixHeight];
  int *valuesG = new int[matrixWidth * matrixHeight];
  int *valuesB = new int[matrixWidth * matrixHeight];
  int *offsets = new int[matrixWidth * matrixHeight];
  int count;
  for (int i = 0;i < matrixWidth;++i)
    for (int j = 0;j < matrixHeight;++j)
      offsets[i + matrixWidth * j] = 4 * (i - borderWidth) +
                                     realWidth1 * (j - borderHeight);

  memcpy(resultImgDataPtr, imageDataPtr, image.byteCount());

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    resultImgDataPtr = backup2 + realWidth2 * i;
    for (int j = 0;j < width;++j)
    {
      int sr, sg, sb, sa;
      int tr = 0, tg = 0, tb = 0;
      count = 0;
      for (int k = 0;k < matrixHeight;++k)
      {
        for (int l = 0;l < matrixWidth;++l)
        {
          if (i + k - borderHeight < 0 ||
              i + k - borderHeight >= height ||
              j + l - borderWidth < 0 ||
              j + l - borderWidth >= width)
            continue;
          int index = k * matrixWidth + l;
          getRGBA(imageDataPtr + offsets[index], sr, sg, sb, sa);
          valuesR[count] = sr;
          valuesG[count] = sg;
          valuesB[count] = sb;
          ++count;
        }
      }
      tr = qBound(0,
                  calculateMidNumber(valuesR, count),
                  MAX_COLOR_VALUE);
      tg = qBound(0,
                  calculateMidNumber(valuesG, count),
                  MAX_COLOR_VALUE);
      tb = qBound(0,
                  calculateMidNumber(valuesB, count),
                  MAX_COLOR_VALUE);
      setRGBA(resultImgDataPtr, tr, tg, tb, MAX_COLOR_VALUE);
      imageDataPtr += 4;
      resultImgDataPtr += 4;
    }
  }
  delete [] valuesR;
  delete [] valuesG;
  delete [] valuesB;
  return resultImg;
}

void ImageAlgorithm::midNumber(QImage *image,
                               int matrixWidth,
                               int matrixHeight)
{
  QImage *result = midNumber(*image, matrixWidth, matrixHeight);
  if (result != NULL)
  {
    memcpy(image->bits(), result->bits(), result->byteCount());
    delete result;
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
