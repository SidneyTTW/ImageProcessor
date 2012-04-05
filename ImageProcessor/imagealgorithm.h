#ifndef IMAGEALGORITHM_H
#define IMAGEALGORITHM_H

#include <QImage>
#include "area.h"
#include "basicstatistic.h"

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

/**
 * Algorithm of images, only supports QImage::Format_ARGB32.
 */
class ImageAlgorithm
{
public:
  /**
   * An enum class to describe the way to calculate the gray scale image.
   */
  enum ImageToGrayAlgorithmType{Green = 0, Float, Integer, Displacement, Average};

  /**
   * An enum class to describe the way to resize image.
   */
  enum ResizeAlgorithmType{NearestNeighbor, Bilinear, Bicubic};

  /**
   * An enum class to describe the RGBA fields.
   */
  enum RGBAFieldFlag{Field_R = 1, Field_G = 2, Field_B = 4};

  /**
   * The type of the RGBA fields.
   */
  typedef QFlags<RGBAFieldFlag> RGBAField;

  /**
   * The only format supported.
   */
  const static QImage::Format SUPPORTED_FORMAT = QImage::Format_ARGB32;

  /**
   * @return Whether the image can be processed by this algorithm.
   */
  static inline bool validType(const QImage& image) {
    return image.format() == SUPPORTED_FORMAT;
  }

  /**
   * Function to get the gray scale at a point.
   *
   * @param r Red.
   * @param g Green.
   * @param b Blue.
   */
  static inline int calculateGray(int r, int g, int b,
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

  /**
   * Convert an image to a gray one.
   *
   * @param image The image to convert.
   * @param type The type of algorithm to use.
   */
  static QImage *convertToGrayScale(const QImage& image,
                                    ImageToGrayAlgorithmType type);

  /**
   * Convert an image to a gray one.
   *
   * @param image The image to convert.
   * @param type The type of algorithm to use.
   */
  static void convertToGrayScale(QImage *image,
                                 ImageToGrayAlgorithmType type);

  /**
   * Convert an image to a black and white.
   *
   * @param image The image to convert.
   * @param threshold The threshold.
   * @param startColor The start color, 0 means black, 1 means white.
   */
  static QImage *convertToBlackAndWhite(const QImage& image,
                                        QVector<int> threshold,
                                        int startColor=0);

  /**
   * Convert an image to a black and white.
   *
   * @param image The image to convert.
   * @param threshold The threshold.
   * @param startColor The start color, 0 means black, 1 means white.
   */
  static void convertToBlackAndWhite(QImage *image,
                                     QVector<int> threshold,
                                     int startColor=0);

  /**
   * Filt an image according to given filter.
   *
   * @param image The image to filt.
   * @param filter The filter.
   */
  template <class T>
  static QImage *filtImage(const QImage& image, const Area& area, T *filter);

  /**
   * Filt an image according to given filter.
   *
   * @param image The image to filt.
   * @param filter The filter.
   */
  template <class T>
  static void filtImage(QImage *image, const Area& area, T *filter);

  /**
   * Reverse the color.
   * The alpha will be remained.
   *
   * @param image The image to reverse.
   */
  static QImage *reverse(const QImage& image);

  /**
   * Reverse the color.
   * The alpha will be remained.
   *
   * @param image The image to reverse.
   */
  static void reverse(QImage *image);

  /**
   * Change rgb tunel.
   * The alpha will be remained.
   *
   * @param image The image.
   * @param mapR The map of red.
   * @param mapG The map of green.
   * @param mapB The map of blue.
   * @param tunel The tunel to change.
   */
  static QImage *changeRGBWithMap(const QImage& image,
                                  int mapR[MAX_COLOR_VALUE],
                                  int mapG[MAX_COLOR_VALUE],
                                  int mapB[MAX_COLOR_VALUE],
                                  RGBAField tunel);

  /**
   * Change rgb tunel.
   * The alpha will be remained.
   *
   * @param image The image.
   * @param mapR The map of red.
   * @param mapG The map of green.
   * @param mapB The map of blue.
   * @param tunel The tunel to change.
   */
  static void changeRGBWithMap(QImage *image,
                               int mapR[MAX_COLOR_VALUE],
                               int mapG[MAX_COLOR_VALUE],
                               int mapB[MAX_COLOR_VALUE],
                               RGBAField tunel);

  /**
   * Resize the image according to the given algorithm type.
   *
   * @param image The image.
   * @param newWidth The new width.
   * @param newHeight The new height.
   * @param type The type of algorithm to use.
   */
  static QImage *resize(const QImage& image,
                        int newWidth,
                        int newHeight,
                        ResizeAlgorithmType type);

  /**
   * Resize the image according to the given algorithm type.
   *
   * @param image The image.
   * @param newWidth The new width.
   * @param newHeight The new height.
   * @param type The type of algorithm to use.
   */
  static void resize(QImage *image,
                     int newWidth,
                     int newHeight,
                     ResizeAlgorithmType type);

  /**
   * Get the statistic of an image.
   *
   * @param image The image to convert.
   * @param type The type of algorithm to use.
   */
  static BasicStatistic getStatistic(const QImage& image,
                                     ImageToGrayAlgorithmType type);


  /**
   * Calculate the throsheld using OTSU.
   *
   * @param image The image.
   * @param type The type of algorithm to use.
   */
  static int OTSU(const QImage& image, ImageToGrayAlgorithmType type);


  /**
   * Calculate the throsheld using max entropy.
   *
   * @param image The image.
   * @param type The type of algorithm to use.
   */
  static int maxEntropy(const QImage& image, ImageToGrayAlgorithmType type);

  /**
   * @param k (2k+1) will be the size of the core.
   * @param d Standard deviation.
   * @param result Pointer of an array(size is (2k+1)(2k+1)).
   */
  static void gaussCore(int k, float d,int *result);

  /**
   * Abstract class of filter.
   * Only stores the width and height.
   * I didn't use virtual function becuase of efficiency.
   * I'm going to use template function which can be optimized by the compiler.
   */
  class AbstractFilter
  {
  public:
    /**
     * Constructor.
     */
    AbstractFilter(int width, int height) :
        _width(width),
        _height(height)
    {
    }

    /**
     * Width of the matrix.
     */
    int _width;

    /**
     * height of the matrix.
     */
    int _height;
  };

  /**
   * Class of filter of convolution.
   */
  class ConvolutionFilter : public AbstractFilter
  {
  public:
    /**
     * Constructor.
     *
     * @param matrix The matrix.
     * @param width Width of the matrix.
     * @param height Height of the matrix.
     * @param divisor The divisor.
     * @param offset The offset.
     */
    ConvolutionFilter(const int *matrix,
                      int width,
                      int height,
                      int divisor,
                      int offset) :
      AbstractFilter(width, height),
      _divisor(divisor),
      _offset(offset)
    {
      _matrix = new int[width * height];
      memcpy(_matrix, matrix, width * height * 4);
    }

    /**
     * Destructor.
     */
    ~ConvolutionFilter()
    {
      delete [] _matrix;
    }

    /**
     * Calculate the RGBA.
     * The alpha will be set to MAX_COLOR_VALUE
     *
     * @param dataPtrPtr Pointer of the pointers of original image data.
     * @param n Number of pointers.
     * @param r Red.
     * @param g Green.
     * @param b Blue.
     * @param a Alpha.
     */
    inline void calculateRGBA(const unsigned char **dataPtrPtr,
                              int n,
                              int& r,
                              int& g,
                              int& b,
                              int& a)
    {
      r = 0;
      g = 0;
      b = 0;
      a = MAX_COLOR_VALUE;
      int sr, sg, sb, sa;
      for (int i = 0;i < n;++i)
      {
        if (dataPtrPtr[i] == NULL)
          continue;
        getRGBA(dataPtrPtr[i], sr, sg, sb, sa);
        r += _matrix[i] * sr;
        g += _matrix[i] * sg;
        b += _matrix[i] * sb;
      }
      r = r / _divisor + _offset;
      g = g / _divisor + _offset;
      b = b / _divisor + _offset;
    }

  private:
    /**
     * The matrix.
     */
    int *_matrix;

    /**
     * The divisor.
     */
    int _divisor;

    /**
     * The offset.
     */
    int _offset;
  };

  /**
   * Class of filter of median.
   */
  class MedianFilter : public AbstractFilter
  {
  public:
    /**
     * Constructor.
     *
     * @param width Width of the matrix.
     * @param height Height of the matrix.
     */
    MedianFilter(int width, int height) :
        AbstractFilter(width, height)
    {
      arrayR = new int[width * height];
      arrayG = new int[width * height];
      arrayB = new int[width * height];
    }

    ~MedianFilter()
    {
      delete [] arrayR;
      delete [] arrayG;
      delete [] arrayB;
    }

    /**
     * Calculate the RGBA.
     * The alpha will be set to MAX_COLOR_VALUE
     *
     * @param dataPtrPtr Pointer of the pointers of original image data.
     * @param n Number of pointers.
     * @param r Red.
     * @param g Green.
     * @param b Blue.
     * @param a Alpha.
     */
    inline void calculateRGBA(const unsigned char **dataPtrPtr,
                              int n,
                              int& r,
                              int& g,
                              int& b,
                              int& a)
    {
      a = MAX_COLOR_VALUE;
      int sr, sg, sb, sa;
      count = 0;
      for (int i = 0;i < n;++i)
      {
        if (dataPtrPtr[i] == NULL)
          continue;
        getRGBA(dataPtrPtr[i], sr, sg, sb, sa);
        arrayR[count] = sr;
        arrayG[count] = sg;
        arrayB[count] = sb;
        ++count;
      }
      r = calculateMidNumber(arrayR, count);
      g = calculateMidNumber(arrayG, count);
      b = calculateMidNumber(arrayB, count);
    }

  private:
    /**
     * Array to store the red values.
     */
    int *arrayR;

    /**
     * Array to store the green values.
     */
    int *arrayG;

    /**
     * Array to store the blue values.
     */
    int *arrayB;

    /**
     * Count of valid numbers.
     */
    int count;
  };

private:
  /**
   * Get RGBA of a point.
   *
   * @param dataPtr The pointer of the data.
   *                The format of the image should be QImage::Format_ARGB32.
   * @param r (Return) Red.
   * @param g (Return) Green.
   * @param b (Return) Blue.
   * @param a (Return) Alpha.
   */
  static inline void getRGBA(const unsigned char * dataPtr,
                             int& r, int& g, int& b, int& a)
  {
    b = *dataPtr;
    g = *(dataPtr+1);
    r = *(dataPtr+2);
    a = *(dataPtr+3);
  }

  /**
   * Set RGBA of a point.
   *
   * @param dataPtr The pointer of the data.
   *                The format of the image should be QImage::Format_ARGB32.
   * @param r Red.
   * @param g Green.
   * @param b Blue.
   * @param a Alpha.
   */
  static inline void setRGBA(unsigned char * dataPtr,
                             int r, int g, int b, int a)
  {
    *dataPtr = (char) b;
    *(dataPtr+1) = (char) g;
    *(dataPtr+2) = (char) r;
    *(dataPtr+3) = (char) a;
  }

  /**
   * Copy RGBA of a point.
   *
   * @param sourceDataPtr The pointer of the source data.
   *                      The format of the image should be
   *                      QImage::Format_ARGB32.
   * @param targetDataPtr  The pointer of the target data.
   *                       The format of the image should be
   *                       QImage::Format_ARGB32.
   */
  static inline void copyRGBA(const unsigned char * sourceDataPtr,
                              unsigned char * targetDataPtr)
  {
    int r, g, b, a;
    getRGBA(sourceDataPtr, r, g, b, a);
    setRGBA(targetDataPtr, r, g, b, a);
  }

  /**
   * Add RGBA of a point.
   *
   * @param sourceDataPtr The pointer of the source data.
   *                      The format of the image should be
   *                      QImage::Format_ARGB32.
   * @param factor The factor.
   * @param targetDataPtr  The pointer of the target data.
   *                       The format of the image should be
   *                       QImage::Format_ARGB32.
   */
  static inline void addRGBA(const unsigned char * sourceDataPtr,
                             double factor,
                             unsigned char * targetDataPtr)
  {
    int sr, sg, sb, sa;
    int tr, tg, tb, ta;
    getRGBA(sourceDataPtr, sr, sg, sb, sa);
    getRGBA(sourceDataPtr, tr, tg, tb, ta);
    tr = qBound(0, (int)(tr + factor * sr), MAX_COLOR_VALUE);
    tg = qBound(0, (int)(tg + factor * sg), MAX_COLOR_VALUE);
    tb = qBound(0, (int)(tb + factor * sb), MAX_COLOR_VALUE);
    ta = qBound(0, (int)(ta + factor * sa), MAX_COLOR_VALUE);
    setRGBA(targetDataPtr, tr, tg, tb, ta);
  }

  /**
   * Add RGBA of a point.
   *
   * @param sourceDataPtr The pointer of the source data.
   *                      The format of the image should be
   *                      QImage::Format_ARGB32.
   * @param factor The factor.
   * @param targetDataPtr  The pointer of the target data.
   *                       The format of the image should be
   *                       QImage::Format_ARGB32.
   */
  static inline void addRGBAs(const unsigned char * sourceDataPtr,
                              double factor,
                              unsigned char * targetDataPtr)
  {
    int sr, sg, sb, sa;
    int tr, tg, tb, ta;
    getRGBA(sourceDataPtr, sr, sg, sb, sa);
    getRGBA(sourceDataPtr, tr, tg, tb, ta);
    tr = qBound(0, (int)(tr + factor * sr), MAX_COLOR_VALUE);
    tg = qBound(0, (int)(tg + factor * sg), MAX_COLOR_VALUE);
    tb = qBound(0, (int)(tb + factor * sb), MAX_COLOR_VALUE);
    ta = qBound(0, (int)(ta + factor * sa), MAX_COLOR_VALUE);
    setRGBA(targetDataPtr, tr, tg, tb, ta);
  }

  /**
   * @param realWidth The bytes in a line.
   * @param x The x position.
   * @param y  The y position.
   * @return The offset from the beginning of data pointer.
   */
  static inline int pixelOffset(int realWidth , int x, int y)
  {
    return realWidth * y + 4 * x;
  }
};

template <class T>
QImage *ImageAlgorithm::filtImage(const QImage& image, const Area& area, T *filter)
{
  if (filter == NULL || !validType(image))
    return NULL;
  if (filter->_width % 2 != 1 || filter->_height % 2 != 1)
    return NULL;
  int matrixWidth = filter->_width;
  int matrixHeight = filter->_height;
  int borderWidth = matrixWidth / 2;
  int borderHeight = matrixHeight / 2;
  int width = image.width();
  int height = image.height();
  const unsigned char *imageDataPtr = image.bits();
  QImage *filtedImg = new QImage(width, height, SUPPORTED_FORMAT);
  unsigned char *filtedImgDataPtr = filtedImg->bits();
  int realWidth1 = image.bytesPerLine();
  int realWidth2 = filtedImg->bytesPerLine();
  const unsigned char *backup1 = imageDataPtr;
  unsigned char *backup2 = filtedImgDataPtr;

  const unsigned char **dataPtrPtr =
      new const unsigned char *[matrixWidth * matrixHeight];
  int *offsets = new int[matrixWidth * matrixHeight];

  for (int i = 0;i < matrixWidth;++i)
    for (int j = 0;j < matrixHeight;++j)
      offsets[i + matrixWidth * j] = 4 * (i - borderWidth) +
                                     realWidth1 * (j - borderHeight);

  memcpy(filtedImgDataPtr, imageDataPtr, image.byteCount());

  for(int i = 0;i < height;++i)
  {
    imageDataPtr = backup1 + realWidth1 * i;
    filtedImgDataPtr = backup2 + realWidth2 * i;
    for (int j = 0;j < width;++j)
    {
      if ((area.getType() == area.TypeEmpty) || area.in(j, i))
      {
        int tr, tg, tb, ta;
        for (int k = 0;k < matrixHeight;++k)
        {
          for (int l = 0;l < matrixWidth;++l)
          {
            int index = k * matrixWidth + l;
            if (i + k - borderHeight < 0 ||
                i + k - borderHeight >= height ||
                j + l - borderWidth < 0 ||
                j + l - borderWidth >= width)
              dataPtrPtr[index] = NULL;
            else
              dataPtrPtr[index] = imageDataPtr + offsets[index];
          }
        }
        filter->calculateRGBA(dataPtrPtr, matrixWidth * matrixHeight, tr, tg, tb, ta);
        tr = qBound(0, tr, MAX_COLOR_VALUE);
        tg = qBound(0, tg, MAX_COLOR_VALUE);
        tb = qBound(0, tb, MAX_COLOR_VALUE);
        ta = qBound(0, ta, MAX_COLOR_VALUE);
        setRGBA(filtedImgDataPtr, tr, tg, tb, ta);
      }
      imageDataPtr += 4;
      filtedImgDataPtr += 4;
    }
  }
  delete [] offsets;
  delete [] dataPtrPtr;
  return filtedImg;
}

template <class T>
void ImageAlgorithm::filtImage(QImage *image, const Area& area, T *filter)
{
  QImage *result = filtImage<T>(*image, area, filter);
  if (result != NULL)
  {
    memcpy(image->bits(), result->bits(), result->byteCount());
    delete result;
  }
}

#endif // IMAGEALGORITHM_H
