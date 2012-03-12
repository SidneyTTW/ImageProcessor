#include "basicstatistic.h"

BasicStatistic::BasicStatistic(int width, int height) :
    _width(width),
    _height(height)
{
  for (int i = 0;i < 4;++i)
    for (int j = 0;j <= MAX_COLOR_VALUE;++j)
      counts[i][j] = 0;
}
