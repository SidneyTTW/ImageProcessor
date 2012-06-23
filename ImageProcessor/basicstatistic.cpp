#include "basicstatistic.h"

BasicStatistic::BasicStatistic() :
    _pixels(0)
{
  for (int i = 0;i < 4;++i)
    for (int j = 0;j <= MAX_COLOR_VALUE;++j)
      counts[i][j] = 0;
}

BasicStatistic::BasicStatistic(int pixels) :
    _pixels(pixels)
{
  for (int i = 0;i < 4;++i)
    for (int j = 0;j <= MAX_COLOR_VALUE;++j)
      counts[i][j] = 0;
}
