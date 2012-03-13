#ifndef PROCESSORAID_H
#define PROCESSORAID_H

#include <QMap>
#include <QVector>

class AbstractImageProcessor;
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

  /**
   * @return Pointers of instances of all kinds of processors.
   */
  static QMap<QString, AbstractImageProcessor *> allProcessor();
};

#endif // PROCESSORAID_H
