#ifndef BASICSTATISTIC_H
#define BASICSTATISTIC_H

#include "myglobal.h"

/**
 * Class to store the basic statistic of an image.
 */
class BasicStatistic
{
public:
  /**
   * An enum class to describe the fields of the statistic.
   */
  enum StatisticField{Red = 0, Green, Blue, Gray};

  /**
   * Constructor.
   */
  BasicStatistic(int width, int height);

  /**
   * Width of the image.
   */
  int _width;

  /**
   * Height of the image.
   */
  int _height;

  /**
   * Array to store the statistic.
   */
  int counts[4][MAX_COLOR_VALUE + 1];
};

#endif // BASICSTATISTIC_H
