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
  static int calculateGray(int r, int g, int b, ImageToGrayAlgorithmType type);

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
  static int OTSU(const QImage& image,
                  ImageToGrayAlgorithmType type);


  /**
   * @brief Calculate the throsheld using max entropy.
   *
   * @param image The image.
   * @param type The type of algorithm to use.
   */
  static int maxEntropy(const QImage& image,
                        ImageToGrayAlgorithmType type);

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
};

#endif // IMAGEALGORITHM_H
