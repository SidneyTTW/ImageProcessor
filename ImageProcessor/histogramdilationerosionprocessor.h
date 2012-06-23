#ifndef HISTOGRAMDILATIONEROSIONPROCESSOR_H
#define HISTOGRAMDILATIONEROSIONPROCESSOR_H

#include "abstractimageprocessorwithdialogoption.h"

#include <QVector>
#include "basicstatistic.h"
#include "myglobal.h"

/**
 * Class of processor to do dilation/erosion operation with histogram.
 */
class HistogramDilationErosionProcessor
{
public:
  /**
   * Types of the operation.
   */
  enum OperationType {Dilation, Erosion};

  /**
   * Constructor.
   */
  HistogramDilationErosionProcessor();

  /**
   * Calculate the map used.
   */
  static void calculateMap(const QVector<int>& offsets,
                           const BasicStatistic& statistic,
                           int map[MAX_COLOR_VALUE + 1],
                           int range,
                           OperationType type);

private:
  /**
   * Offsets.
   */
  QVector<int> offsets;

  /**
   * The range effected by the offsets.
   */
  int range;

  /**
   * The type.
   */
  OperationType type;
};

#endif // HISTOGRAMDILATIONEROSIONPROCESSOR_H
