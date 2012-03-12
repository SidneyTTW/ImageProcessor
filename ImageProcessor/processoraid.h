#ifndef PROCESSORAID_H
#define PROCESSORAID_H

#include <QVector>

class AbstractImageProcessorWithSimpleOption;
class AbstractImageProcessorWithDialogOption;

/**
 * Class with some aid functions.
 */
class ProcessorAid
{
public:
  /**
   * @return Pointers of instances of simple options.
   */
  static QVector<AbstractImageProcessorWithSimpleOption *> simpleOptions();

  /**
   * @return Pointers of instances of complex options.
   */
  static QVector<AbstractImageProcessorWithDialogOption *> complexOptions();
};

#endif // PROCESSORAID_H
