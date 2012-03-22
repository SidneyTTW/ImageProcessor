#ifndef IMAGEALGORITHM_H
#define IMAGEALGORITHM_H

#include <QImage>
#include "basicstatistic.h"

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
   * Convolution.
   * The alpha will be set to MAX_COLOR_VALUE
   *
   * @param image The image to convert.
   * @param matrix The matrix.
   * @param matrixWidth Width of the matrix.
   * @param divisor The divisor.
   * @param offset The offset.
   */
  static QImage *convolution(const QImage& image,
                             const QVector<int>& matrix,
                             int matrixWidth,
                             int divisor,
                             int offset);

  /**
   * Convolution.
   * The alpha will be set to MAX_COLOR_VALUE
   *
   * @param image The image to convert.
   * @param matrix The matrix.
   * @param matrixWidth Width of the matrix.
   * @param divisor The divisor.
   * @param offset The offset.
   */
  static void convolution(QImage *image,
                          const QVector<int>& matrix,
                          int matrixWidth,
                          int divisor,
                          int offset);

  /**
   * Filter using mid number.
   * The alpha will be set to MAX_COLOR_VALUE
   *
   * @param image The image to convert.
   * @param matrixWidth Width of the matrix.
   * @param matrixHeight Height of the matrix.
   */
  static QImage *midNumber(const QImage& image,
                           int matrixWidth,
                           int matrixHeight);

  /**
   * Filter using mid number.
   * The alpha will be set to MAX_COLOR_VALUE
   *
   * @param image The image to convert.
   * @param matrixWidth Width of the matrix.
   * @param matrixHeight Height of the matrix.
   */
  static void midNumber(QImage *image, int matrixWidth, int matrixHeight);

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
   * @brief Change rgb tunel.
   * The alpha will be remained.
   *
   * @param image The image.
   * @param map The map.
   * @param tunel The tunel to change.
   */
  static QImage *changeRGBWithMap(const QImage& image,
                                  int mapR[MAX_COLOR_VALUE],
                                  int mapG[MAX_COLOR_VALUE],
                                  int mapB[MAX_COLOR_VALUE],
                                  RGBAField tunel);

  /**
   * @brief Change rgb tunel.
   * The alpha will be remained.
   *
   * @param image The image.
   * @param map The map.
   * @param tunel The tunel to change.
   */
  static void changeRGBWithMap(QImage *image,
                               int mapR[MAX_COLOR_VALUE],
                               int mapG[MAX_COLOR_VALUE],
                               int mapB[MAX_COLOR_VALUE],
                               RGBAField tunel);

  /**
   * Get the statistic of an image.
   *
   * @param image The image to convert.
   * @param type The type of algorithm to use.
   */
  static BasicStatistic getStatistic(const QImage& image,
                                     ImageToGrayAlgorithmType type);


  /**
   * @brief Calculate the throsheld using OTSU.
   *
   * @param image The image.
   * @param type The type of algorithm to use.
   */
  static int OTSU(const QImage& image, ImageToGrayAlgorithmType type);


  /**
   * @brief Calculate the throsheld using max entropy.
   *
   * @param image The image.
   * @param type The type of algorithm to use.
   */
  static int maxEntropy(const QImage& image, ImageToGrayAlgorithmType type);

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
  static inline void pixelOffset(int realWidth , int x, int y)
  {
    return realWidth * y + 4 * x;
  }
};

#endif // IMAGEALGORITHM_H
