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
   * @param dataPtr The pointer of the data. The format of the image should be QImage::Format_ARGB32.
   * @param r (Return) Red.
   * @param g (Return) Green.
   * @param b (Return) Blue.
   * @param a (Return) Alpha.
   */
  static void getRGBA(const unsigned char * dataPtr, int& r, int& g, int& b, int& a);
};

#endif // IMAGEALGORITHM_H
