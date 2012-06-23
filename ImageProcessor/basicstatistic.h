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
  BasicStatistic();

  /**
   * Constructor.
   */
  BasicStatistic(int pixels);

  /**
   * Number of pixels.
   */
  int _pixels;

  /**
   * Array to store the statistic.
   */
  int counts[4][MAX_COLOR_VALUE + 1];
};

#endif // BASICSTATISTIC_H
