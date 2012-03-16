#include "imagealgorithm.h"

#include <qmath.h>

int ImageAlgorithm::calculateGray(int r, int g, int b,
                                  ImageToGrayAlgorithmType type)
{
  switch (type)
  {
  case Green:
    return g;
    break;
  case Float:
    return 0.11 * b + 0.59 * g + 0.3 * r;
    break;
  case Integer:
    return (11 * b + 59 * g + 30 * r) / 100;
    break;
  case Displacement:
    return (77 * b + 151 * g + 28 * r) >> 8;
    break;
  case Average:
    return (b + g + r) / 3;
    break;
  default:
    return 0;
    break;
  }
}

// Thanks to http://blog.csdn.net/nrc_douningbo/article/details/5929106
// Although there was something wrong, my program was sped up a lot.
QImage *ImageAlgorithm::convertToGrayScale(const QImage& image,
                                           ImageToGrayAlgorithmType type)
{
  if (!validType(image))
    return NULL;
  QSize imageSize = image.size();
  int width = imageSize.width();
  int height = imageSize.height();
  const unsigned char *imageDataPtr = image.bits();
  QImage *grayImg = new QImage(imageSize, SUPPORTED_FORMAT);
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
      for (int k = 0;k < 3;++k)
        *(grayImgDataPtr + k) = gray;
      *(grayImgDataPtr + 3) = MAX_COLOR_VALUE;
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
  QSize imageSize = image->size();
  int width = imageSize.width();
  int height = imageSize.height();
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
  QSize imageSize = image.size();
  int width = imageSize.width();
  int height = imageSize.height();
  const unsigned char *imageDataPtr = image.bits();
  QImage *blackAndWhiteImg = new QImage(imageSize, SUPPORTED_FORMAT);
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
      for (int k = 0;k < 3;++k)
        *(blackAndWhiteImgDataPtr + k) = coloreMap[gray];
      *(blackAndWhiteImgDataPtr + 3) = MAX_COLOR_VALUE;
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
  QSize imageSize = image->size();
  int width = imageSize.width();
  int height = imageSize.height();
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

BasicStatistic ImageAlgorithm::getStatistic(const QImage& image,
                                            ImageToGrayAlgorithmType type)
{
  BasicStatistic result(image.width(), image.height());
  if (!validType(image))
    return result;
  QSize imageSize = image.size();
  int width = imageSize.width();
  int realWidth1 = image.bytesPerLine();
  int height = imageSize.height();
  const unsigned char *imageDataPtr = image.bits();
  const unsigned char *backup1 = imageDataPtr;
  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
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
