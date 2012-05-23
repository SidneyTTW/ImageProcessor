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
namespace ImageAlgorithm
{
  /**
   * An enum class to describe the way to calculate the gray scale image.
   */
  enum ImageToGrayAlgorithmType{Green = 0, Float, Integer, Displacement, Average};

  /**
   * An enum class to describe the way to geometric operator.
   */
  enum GeometricAlgorithmType{NearestNeighbor, Bilinear, Bicubic};

  /**
   * An enum class to describe the RGBA fields.
   */
  enum RGBAFieldFlag{Field_R = 1, Field_G = 2, Field_B = 4};

  /**
   * An enum class to describe the algebra operation.
   */
  enum AlgebraOperationType{Add, Minus, Multiply, Divide};

  /**
   * The type of the RGBA fields.
   */
  typedef QFlags<RGBAFieldFlag> RGBAField;

  /**
   * The only format supported.
   */
  const QImage::Format SUPPORTED_FORMAT = QImage::Format_ARGB32;

  /**
   * @return Whether the image can be processed by this algorithm.
   */
  inline bool validType(const QImage& image) {
    return image.format() == SUPPORTED_FORMAT;
  }

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
  inline void getRGBA(const unsigned char * dataPtr,
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
  inline void setRGBA(unsigned char * dataPtr,
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
  inline void copyRGBA(const unsigned char * sourceDataPtr,
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
  inline void addRGBA(const unsigned char * sourceDataPtr,
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
  inline void addRGBAs(const unsigned char * sourceDataPtr,
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
  inline int pixelOffset(int realWidth , int x, int y)
  {
    return realWidth * y + 4 * x;
  }

  /**
   * Function to get the gray scale at a point.
   *
   * @param r Red.
   * @param g Green.
   * @param b Blue.
   */
  inline int calculateGray(int r, int g, int b,
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
   * Function to get the HSL from RGB.
   * Thanks to http://blog.csdn.net/aniven/article/details/2205851
   *
   * @param r Red.
   * @param g Green.
   * @param b Blue.
   * @param h Red.
   * @param s Green.
   * @param l Blue.
   */
  inline void RGB2HSL(int r, int g, int b, double& h,double& s,double& l)
  {
    double dr, dg, db, max, min, del_R, del_G, del_B, del_Max;
    dr = (double)r / MAX_COLOR_VALUE;
    dg = (double)g / MAX_COLOR_VALUE;
    db = (double)b / MAX_COLOR_VALUE;

    min = qMin(dr, qMin(dg, db));
    max = qMax(dr, qMax(dg, db));
    del_Max = max - min;

    l = (max + min) / 2.0;

    if (del_Max == 0)
    {
      h = 0;
      s = 0;
    }
    else
    {
      if (l < 0.5)
        s = del_Max / (max + min);
      else
        s = del_Max / (2 - max - min);

      del_R = (((max - dr) / 6.0) + (del_Max / 2.0)) / del_Max;
      del_G = (((max - dg) / 6.0) + (del_Max / 2.0)) / del_Max;
      del_B = (((max - db) / 6.0) + (del_Max / 2.0)) / del_Max;

      if (dr == max)
        h = del_B - del_G;
      else if (dg == max)
        h = (1.0 / 3.0) + del_R - del_B;
      else if (db == max)
        h = (2.0 / 3.0) + del_G - del_R;

      if (h < 0)
        h += 1;
      if (h > 1)
        h -= 1;
    }
  }

  /**
   * Aid function used to do the convertion from HSL to RGB.
   * Thanks to http://blog.csdn.net/aniven/article/details/2205851
   *
   * @param v1 A value.
   * @param v2 Another value.
   * @param vH Hue.
   */
  inline double Hue2RGB(double v1, double v2, double vH)
  {
    if (vH < 0)
      vH += 1;
    if (vH > 1)
      vH -= 1;
    if (6.0 * vH < 1)
      return v1 + (v2 - v1) * 6.0 * vH;
    if (2.0 * vH < 1)
      return v2;
    if (3.0 * vH < 2)
      return v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0;
    return (v1);
  }

  /**
   * Function to get the RGB from HSL.
   * Thanks to http://blog.csdn.net/aniven/article/details/2205851
   *
   * @param h Red.
   * @param s Green.
   * @param l Blue.
   * @param r Red.
   * @param g Green.
   * @param b Blue.
   */
  inline void HSL2RGB(double h,double s,double l, int& r, int& g, int& b)
  {
    double var_1, var_2;
    if (s == 0)
    {
        r = l * MAX_COLOR_VALUE;
        g = l * MAX_COLOR_VALUE;
        b = l * MAX_COLOR_VALUE;
    }
    else
    {
      if (l < 0.5)
        var_2 = l * (1 + s);
      else
        var_2 = (l + s) - (s * l);

      var_1 = 2.0 * l - var_2;

      r = 255.0 * Hue2RGB(var_1, var_2, h + (1.0 / 3.0));
      g = 255.0 * Hue2RGB(var_1, var_2, h);
      b = 255.0 * Hue2RGB(var_1, var_2, h - (1.0 / 3.0));
    }
  }

  /**
   * Convert an image to a gray one.
   *
   * @param image The image to convert.
   * @param type The type of algorithm to use.
   */
   QImage *convertToGrayScale(const QImage& image,
                                    ImageToGrayAlgorithmType type);

  /**
   * Convert an image to a gray one.
   *
   * @param image The image to convert.
   * @param type The type of algorithm to use.
   */
  void convertToGrayScale(QImage *image,
                                 ImageToGrayAlgorithmType type);

  /**
   * Convert an image to a black and white.
   *
   * @param image The image to convert.
   * @param threshold The threshold.
   * @param startColor The start color, 0 means black, 1 means white.
   */
  QImage *convertToBlackAndWhite(const QImage& image,
                                        QVector<int> threshold,
                                        int startColor=0);

  /**
   * Convert an image to a black and white.
   *
   * @param image The image to convert.
   * @param threshold The threshold.
   * @param startColor The start color, 0 means black, 1 means white.
   */
  void convertToBlackAndWhite(QImage *image,
                                     QVector<int> threshold,
                                     int startColor=0);

  /**
   * Filt an image according to given filter.
   *
   * @param image The image to filt.
   * @param filter The filter.
   */
  template <class T>
  QImage *filtImage(const QImage& image, const Area& area, T *filter);

  /**
   * Filt an image according to given filter.
   *
   * @param image The image to filt.
   * @param filter The filter.
   */
  template <class T>
  void filtImage(QImage *image, const Area& area, T *filter);

  /**
   * Reverse the color.
   * The alpha will be remained.
   *
   * @param image The image to reverse.
   */
  QImage *reverse(const QImage& image);

  /**
   * Reverse the color.
   * The alpha will be remained.
   *
   * @param image The image to reverse.
   */
  void reverse(QImage *image);

  /**
   * Change rgb tunel.
   * The alpha will be remained.
   *
   * @param image The image.
   * @param mapR The map of red.
   * @param mapG The map of green.
   * @param mapB The map of blue.
   * @param tunel The tunel to change.
   * @param area The area to change.
   */
  QImage *changeRGBWithMap(const QImage& image,
                           const int mapR[MAX_COLOR_VALUE + 1],
                           const int mapG[MAX_COLOR_VALUE + 1],
                           const int mapB[MAX_COLOR_VALUE + 1],
                           RGBAField tunel,
                           Area area = Area());

  /**
   * Change rgb tunel.
   * The alpha will be remained.
   *
   * @param image The image.
   * @param mapR The map of red.
   * @param mapG The map of green.
   * @param mapB The map of blue.
   * @param tunel The tunel to change.
   * @param area The area to change.
   */
  void changeRGBWithMap(QImage *image,
                        const int mapR[MAX_COLOR_VALUE + 1],
                        const int mapG[MAX_COLOR_VALUE + 1],
                        const int mapB[MAX_COLOR_VALUE + 1],
                        RGBAField tunel,
                        Area area = Area());

  /**
   * Resize the image according to the given algorithm type.
   *
   * @param image The image.
   * @param area The area.
   * @param newWidth The new width.
   * @param newHeight The new height.
   * @param type The type of algorithm to use.
   */
  QImage *resize(const QImage& image,
                 Area area,
                 int newWidth,
                 int newHeight,
                 GeometricAlgorithmType type);

  /**
   * Resize the image according to the given algorithm type.
   *
   * @param image The image.
   * @param area The area.
   * @param newWidth The new width.
   * @param newHeight The new height.
   * @param type The type of algorithm to use.
   */
  void resize(QImage *image,
              Area area,
              int newWidth,
              int newHeight,
              GeometricAlgorithmType type);

  /**
   * Change the HSL using given offsets.
   * The alpha will be remained.
   *
   * @param image The image to reverse.
   * @param hOffset Offset of h.
   * @param sOffset Offset of s.
   * @param lOffset Offset of l.
   */
  QImage *changeHSL(const QImage& image,
                    double hOffset,
                    double sOffset,
                    double lOffset);

  /**
   * Change the HSL using given offsets.
   * The alpha will be remained.
   *
   * @param image The image to reverse.
   * @param hOffset Offset of h.
   * @param sOffset Offset of s.
   * @param lOffset Offset of l.
   */
  void changeHSL(QImage *image,
                 double hOffset,
                 double sOffset,
                 double lOffset);

  /**
   * Red eye reduction.
   *
   * @param image The image to do the red eye reduction.
   * @param area The area of eye.
   */
  QImage *redEyeReduction(const QImage& image, const Area& area);

  /**
   * Red eye reduction.
   *
   * @param image The image to do the red eye reduction.
   * @param area The area of eye.
   */
  void redEyeReduction(QImage *image, const Area& area);

  /**
   * Algebra operation with another image.
   *
   * @param image One of the image to do the algebra operation.
   * @param image Another image to do the algebra operation.
   * @param type The type of the algorithm.
   */
  QImage *algebraOperation(const QImage& image,
                           const QImage& image2,
                           AlgebraOperationType type);

  /**
   * Algebra operation with another image.
   *
   * @param image One of the image to do the algebra operation.
   * @param image Another image to do the algebra operation.
   * @param type The type of the algorithm.
   */
  void algebraOperation(QImage *image,
                        const QImage& image2,
                        AlgebraOperationType type);

  /**
   * Rotate.
   *
   * @param image The image to rotate.
   * @param angle The angle to rotate.
   * @param area The area to rotate.
   */
  QImage *rotate(const QImage& image,
                 double angle,
                 GeometricAlgorithmType type,
                 const Area& area=Area());

  /**
   * Rotate.
   *
   * @param image The image to rotate.
   * @param angle The angle to rotate.
   * @param area The area to rotate.
   */
  void rotate(QImage *image,
              double angle,
              GeometricAlgorithmType type,
              const Area& area=Area());

  /**
   * Erase an area with given color.
   *
   * @param image The image to do the red eye reduction.
   * @param area The area of eye.
   * @param color The color to set.
   */
  QImage *erase(const QImage& image, const Area& area, const QColor& color);

  /**
   * Erase an area with given color.
   *
   * @param image The image to do the red eye reduction.
   * @param area The area of eye.
   * @param color The color to set.
   */
  void erase(QImage *image, const Area& area, const QColor& color);

  /**
   * Erase a point and nearby points if their color are similar.
   *
   * @param image The image to do the red eye reduction.
   * @param pos The position.
   * @param allowedDifference The allowed difference.
   */
  QImage *magicErase(const QImage& image, const QPoint& pos, int allowedDifference);

  /**
   * Erase a point and nearby points if their color are similar.
   *
   * @param image The image to do the red eye reduction.
   * @param pos The position.
   * @param allowedDifference The allowed difference.
   */
  void magicErase(QImage *image, const QPoint& pos, int allowedDifference);

  /**
   * Get the statistic of an image.
   *
   * @param image The image to convert.
   * @param type The type of algorithm to use.
   */
  BasicStatistic getStatistic(const QImage& image,
                              ImageToGrayAlgorithmType type);


  /**
   * Calculate the throsheld using OTSU.
   *
   * @param image The image.
   * @param type The type of algorithm to use.
   */
  int OTSU(const QImage& image, ImageToGrayAlgorithmType type);


  /**
   * Calculate the throsheld using max entropy.
   *
   * @param image The image.
   * @param type The type of algorithm to use.
   */
  int maxEntropy(const QImage& image, ImageToGrayAlgorithmType type);

  /**
   * @param k (2k+1) will be the size of the core.
   * @param d Standard deviation.
   * @param result Pointer of an array(size is (2k+1)(2k+1)).
   */
  void gaussCore(int k, float d,int *result);

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
     * Filt the image.
     * The alpha will be set to MAX_COLOR_VALUE
     *
     * @param imageDataPtr Pointer of the original image.
     * @param filtedImgDataPtr Pointer of the filted image.
     * @param offsets Pointer of the offsets.
     * @param isNull Pointer of whether the pixel is null.
     * @param n Number of pointers.
     */
    inline void filt(const unsigned char *imageDataPtr,
                     unsigned char *filtedImgDataPtr,
                     int *offsets,
                     bool *isNull,
                     int n)
    {
      if (isNull[n / 2])
        return;
      int r = 0;
      int g = 0;
      int b = 0;
      int a = MAX_COLOR_VALUE;
      int sr, sg, sb, sa;
      for (int i = 0;i < n;++i)
      {
        if (isNull[i])
          continue;
        getRGBA(imageDataPtr + offsets[i], sr, sg, sb, sa);
        r += _matrix[i] * sr;
        g += _matrix[i] * sg;
        b += _matrix[i] * sb;
      }
      r = r / _divisor + _offset;
      g = g / _divisor + _offset;
      b = b / _divisor + _offset;

      r = qBound(0, r, MAX_COLOR_VALUE);
      g = qBound(0, g, MAX_COLOR_VALUE);
      b = qBound(0, b, MAX_COLOR_VALUE);
      a = qBound(0, a, MAX_COLOR_VALUE);
      setRGBA(filtedImgDataPtr + offsets[n / 2], r, g, b, a);
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
     * Filt the image.
     * The alpha will be set to MAX_COLOR_VALUE
     *
     * @param imageDataPtr Pointer of the original image.
     * @param filtedImgDataPtr Pointer of the filted image.
     * @param offsets Pointer of the offsets.
     * @param isNull Pointer of whether the pixel is null.
     * @param n Number of pointers.
     */
    inline void filt(const unsigned char *imageDataPtr,
                     unsigned char *filtedImgDataPtr,
                     int *offsets,
                     bool *isNull,
                     int n)
    {
      int sr, sg, sb, sa;
      count = 0;
      for (int i = 0;i < n;++i)
      {
        if (isNull[i])
          continue;
        getRGBA(imageDataPtr + offsets[i], sr, sg, sb, sa);
        arrayR[count] = sr;
        arrayG[count] = sg;
        arrayB[count] = sb;
        ++count;
      }
      int r = qBound(0, calculateMidNumber(arrayR, count), MAX_COLOR_VALUE);
      int g = qBound(0, calculateMidNumber(arrayG, count), MAX_COLOR_VALUE);
      int b = qBound(0, calculateMidNumber(arrayB, count), MAX_COLOR_VALUE);
      int a = MAX_COLOR_VALUE;
      setRGBA(filtedImgDataPtr + offsets[n / 2], r, g, b, a);
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

  /**
   * Class of filter of dilation.
   */
  class DilationFilter : public AbstractFilter
  {
  public:
    /**
     * Constructor.
     *
     * @param matrix The matrix.
     * @param center Center of the matrix.
     * @param width Width of the matrix.
     * @param height Height of the matrix.
     * @param divisor The divisor.
     * @param offset The offset.
     */
    DilationFilter(const int *matrix,
                   int center,
                   int width,
                   int height) :
      AbstractFilter(width, height),
      _center(center)
    {
      _matrix = new int[width * height];
      memcpy(_matrix, matrix, width * height * 4);
    }

    /**
     * Destructor.
     */
    ~DilationFilter()
    {
      delete [] _matrix;
    }

    /**
     * Filt the image.
     * The alpha will be set to MAX_COLOR_VALUE
     *
     * @param imageDataPtr Pointer of the original image.
     * @param filtedImgDataPtr Pointer of the filted image.
     * @param offsets Pointer of the offsets.
     * @param isNull Pointer of whether the pixel is null.
     * @param n Number of pointers.
     */
    inline void filt(const unsigned char *imageDataPtr,
                     unsigned char *filtedImgDataPtr,
                     int *offsets,
                     bool *isNull,
                     int n)
    {
      int r, g, b, a;
      if (isNull[_center])
        return;
      getRGBA(imageDataPtr + offsets[_center], r, g, b, a);

      int sr, sg, sb, sa;
      bool touch = false;
      for (int i = 0;i < n;++i)
      {
        if (isNull[i])
          continue;
        getRGBA(imageDataPtr + offsets[i], sr, sg, sb, sa);
        if (_matrix[i] != 0 && _matrix[i] <= sg)
        {
          touch = true;
          break;
        }
      }
      if (touch)
        setRGBA(filtedImgDataPtr + offsets[_center],
                MAX_COLOR_VALUE,
                MAX_COLOR_VALUE,
                MAX_COLOR_VALUE,
                MAX_COLOR_VALUE);
      else
        setRGBA(filtedImgDataPtr + offsets[_center], 0, 0, 0, MAX_COLOR_VALUE);
    }

  private:
    /**
     * The matrix.
     */
    int *_matrix;

    /**
     * The center.
     */
    int _center;
  };

  /**
   * Class of filter of dilation for gray image.
   */
  class GrayDilationFilter : public AbstractFilter
  {
  public:
    /**
     * Constructor.
     *
     * @param matrix The matrix.
     * @param center Center of the matrix.
     * @param width Width of the matrix.
     * @param height Height of the matrix.
     * @param divisor The divisor.
     * @param offset The offset.
     */
    GrayDilationFilter(const int *matrix,
                       int center,
                       int width,
                       int height) :
      AbstractFilter(width, height),
      _center(center)
    {
      _matrix = new int[width * height];
      memcpy(_matrix, matrix, width * height * 4);
    }

    /**
     * Destructor.
     */
    ~GrayDilationFilter()
    {
      delete [] _matrix;
    }

    /**
     * Filt the image.
     * The alpha will be set to MAX_COLOR_VALUE
     *
     * @param imageDataPtr Pointer of the original image.
     * @param filtedImgDataPtr Pointer of the filted image.
     * @param offsets Pointer of the offsets.
     * @param isNull Pointer of whether the pixel is null.
     * @param n Number of pointers.
     */
    inline void filt(const unsigned char *imageDataPtr,
                     unsigned char *filtedImgDataPtr,
                     int *offsets,
                     bool *isNull,
                     int n)
    {
      int r, g, b, a;
      if (isNull[_center])
        return;
      getRGBA(imageDataPtr + offsets[_center], r, g, b, a);

      int sr, sg, sb, sa;
      int value = 0;
      for (int i = 0;i < n;++i)
      {
        if (isNull[i])
          continue;
        getRGBA(imageDataPtr + offsets[i], sr, sg, sb, sa);
        value = qBound(value, _matrix[i] + sg, MAX_COLOR_VALUE);
      }
      setRGBA(filtedImgDataPtr + offsets[_center],
              value,
              value,
              value,
              MAX_COLOR_VALUE);
    }

  private:
    /**
     * The matrix.
     */
    int *_matrix;

    /**
     * The center.
     */
    int _center;
  };

  /**
   * Class of filter of erosion.
   */
  class ErosionFilter : public AbstractFilter
  {
  public:
    /**
     * Constructor.
     *
     * @param matrix The matrix.
     * @param center Center of the matrix.
     * @param width Width of the matrix.
     * @param height Height of the matrix.
     */
    ErosionFilter(const int *matrix,
                  int center,
                  int width,
                  int height) :
      AbstractFilter(width, height),
      _center(center)
    {
      _matrix = new int[width * height];
      memcpy(_matrix, matrix, width * height * 4);
    }

    /**
     * Destructor.
     */
    ~ErosionFilter()
    {
      delete [] _matrix;
    }

    /**
     * Filt the image.
     * The alpha will be set to MAX_COLOR_VALUE
     *
     * @param imageDataPtr Pointer of the original image.
     * @param filtedImgDataPtr Pointer of the filted image.
     * @param offsets Pointer of the offsets.
     * @param isNull Pointer of whether the pixel is null.
     * @param n Number of pointers.
     */
    inline void filt(const unsigned char *imageDataPtr,
                     unsigned char *filtedImgDataPtr,
                     int *offsets,
                     bool *isNull,
                     int n)
    {
      int r, g, b, a;
      if (isNull[_center])
        return;
      getRGBA(imageDataPtr + offsets[_center], r, g, b, a);
      a = MAX_COLOR_VALUE;

      int sr, sg, sb, sa;
      bool allIn = true;
      for (int i = 0;i < n;++i)
      {
        if (isNull[i])
          continue;
        getRGBA(imageDataPtr + offsets[i], sr, sg, sb, sa);
        if (_matrix[i] != 0 && _matrix[i] > sg)
        {
          allIn = false;
          break;
        }
      }
      if (allIn)
        setRGBA(filtedImgDataPtr + offsets[_center],
                MAX_COLOR_VALUE,
                MAX_COLOR_VALUE,
                MAX_COLOR_VALUE,
                MAX_COLOR_VALUE);
      else
        setRGBA(filtedImgDataPtr + offsets[_center], 0, 0, 0, MAX_COLOR_VALUE);
    }

  private:
    /**
     * The matrix.
     */
    int *_matrix;

    /**
     * The center.
     */
    int _center;
  };

  /**
   * Class of filter of erosion.
   */
  class GrayErosionFilter : public AbstractFilter
  {
  public:
    /**
     * Constructor.
     *
     * @param matrix The matrix.
     * @param center Center of the matrix.
     * @param width Width of the matrix.
     * @param height Height of the matrix.
     */
    GrayErosionFilter(const int *matrix,
                      int center,
                      int width,
                      int height) :
      AbstractFilter(width, height),
      _center(center)
    {
      _matrix = new int[width * height];
      memcpy(_matrix, matrix, width * height * 4);
    }

    /**
     * Destructor.
     */
    ~GrayErosionFilter()
    {
      delete [] _matrix;
    }

    /**
     * Filt the image.
     * The alpha will be set to MAX_COLOR_VALUE
     *
     * @param imageDataPtr Pointer of the original image.
     * @param filtedImgDataPtr Pointer of the filted image.
     * @param offsets Pointer of the offsets.
     * @param isNull Pointer of whether the pixel is null.
     * @param n Number of pointers.
     */
    inline void filt(const unsigned char *imageDataPtr,
                     unsigned char *filtedImgDataPtr,
                     int *offsets,
                     bool *isNull,
                     int n)
    {
      int r, g, b, a;
      if (isNull[_center])
        return;
      getRGBA(imageDataPtr + offsets[_center], r, g, b, a);
      a = MAX_COLOR_VALUE;

      int sr, sg, sb, sa;
      int value = MAX_COLOR_VALUE;
      for (int i = 0;i < n;++i)
      {
        if (isNull[i])
          continue;
        getRGBA(imageDataPtr + offsets[i], sr, sg, sb, sa);
        value = qBound(0, sg - _matrix[i], value);
      }
      setRGBA(filtedImgDataPtr + offsets[_center],
              value,
              value,
              value,
              MAX_COLOR_VALUE);
    }

  private:
    /**
     * The matrix.
     */
    int *_matrix;

    /**
     * The center.
     */
    int _center;
  };
}

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

  bool *isNull = new bool[matrixWidth * matrixHeight];
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
        for (int k = 0;k < matrixHeight;++k)
        {
          for (int l = 0;l < matrixWidth;++l)
          {
            int index = k * matrixWidth + l;
            isNull[index] =  i + k - borderHeight < 0 ||
                             i + k - borderHeight >= height ||
                             j + l - borderWidth < 0 ||
                             j + l - borderWidth >= width;
          }
        }
        filter->filt(imageDataPtr,
                     filtedImgDataPtr,
                     offsets,
                     isNull,
                     matrixWidth * matrixHeight);
      }
      imageDataPtr += 4;
      filtedImgDataPtr += 4;
    }
  }
  delete [] isNull;
  delete [] offsets;
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
