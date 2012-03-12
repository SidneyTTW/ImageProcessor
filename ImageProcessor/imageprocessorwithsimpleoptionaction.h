#ifndef IMAGEPROCESSORWITHSIMPLEOPTIONACTION_H
#define IMAGEPROCESSORWITHSIMPLEOPTIONACTION_H

#include <QAction>

class AbstractImageProcessorWithSimpleOption;

/**
 * Class of action to wrap image processor with simple option.
 */
class ImageProcessorWithSimpleOptionAction : public QAction
{
public:
  /**
   * Constructor.
   *
   * @param configurationInstance One instance of the processor to set
   * configuration.
   */
  ImageProcessorWithSimpleOptionAction(AbstractImageProcessorWithSimpleOption *
                                       configurationInstance,
                                       QObject *parent);

  /**
   * @return The instance of the processor to set configuration.
   */
  inline AbstractImageProcessorWithSimpleOption * getConfiguarionInstance()
  {
    return _configurationInstance;
  }

private:
  /**
   * The one instance of the processor to set configuration.
   */
  AbstractImageProcessorWithSimpleOption *_configurationInstance;
};

#endif // IMAGEPROCESSORWITHSIMPLEOPTIONACTION_H
