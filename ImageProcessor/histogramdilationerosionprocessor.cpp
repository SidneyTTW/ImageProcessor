#include "histogramdilationerosionprocessor.h"

HistogramDilationErosionProcessor::HistogramDilationErosionProcessor()
{
}

void HistogramDilationErosionProcessor::calculateMap(const QVector<int>& offsets,
                                                     const BasicStatistic& statistic,
                                                     int map[MAX_COLOR_VALUE + 1],
                                                     int range,
                                                     OperationType type)
{
  int center = offsets.size() / 2;
  int add = type == Dilation ? 1 : -1;
  for (int i = 0;i <= MAX_COLOR_VALUE;++i)
  {


  }

}
