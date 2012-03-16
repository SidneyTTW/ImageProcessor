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
                                    int divisor,
                                    int offset)
{
  int matrixSize = 0;
  for (int i = 0;i < matrix.size();++i)
  {
    if ((2 * i + 1) * (2 * i + 1) == matrix.size())
    {
      matrixSize = 2 * i + 1;
      break;
    }
  }
  if (matrixSize == 0)
    return NULL;
  if (!validType(image))
    return NULL;
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
  for (int i = 0;i < matrixSize;++i)
    for (int j = 0;j < matrixSize;++j)
      offsets[matrixSize * i + j] = realWidth1 * (i - matrixSize / 2) +
                                    4 * (j - matrixSize / 2);

  memcpy(convolutionImgDataPtr, imageDataPtr, width * 4);
  if (height > 1)
    memcpy(convolutionImgDataPtr + realWidth1 * (height - 1),
           imageDataPtr + realWidth2 * (height - 1),
           width * 4);

  for(int i = 1;i < height - 1;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    convolutionImgDataPtr = backup2 + realWidth2 * i;
    memcpy(convolutionImgDataPtr, imageDataPtr, 4);
    for (int j = 1;j < width - 1;++j)
    {
      int sr, sg, sb, sa;
      int tr = 0, tg = 0, tb = 0;
      for (int k = 0;k < matrix.size();++k)
      {
        getRGBA(imageDataPtr + offsets[k], sr, sg, sb, sa);
        tr += factors[k] * sr;
        tg += factors[k] * sg;
        tb += factors[k] * sb;
      }
      tr = tr / divisor + offset;
      tg = tg / divisor + offset;
      tb = tb / divisor + offset;
      setRGBA(convolutionImgDataPtr, tr, tg, tb, MAX_COLOR_VALUE);
      imageDataPtr += 4;
      convolutionImgDataPtr += 4;
    }
    if (width > 1)
      memcpy(convolutionImgDataPtr, imageDataPtr, 4);
  }
  return convolutionImg;
}

void ImageAlgorithm::convolution(QImage *image,
                                 const QVector<int>& matrix,
                                 int divisor,
                                 int offset)
{
  QImage *result = convolution(*image, matrix, divisor, offset);
  memcpy(image->bits(), result->bits(), result->byteCount());
  delete result;
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
