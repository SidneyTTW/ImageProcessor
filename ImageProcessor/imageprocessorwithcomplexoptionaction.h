#ifndef IMAGEPROCESSORWITHCOMPLEXOPTIONACTION_H
#define IMAGEPROCESSORWITHCOMPLEXOPTIONACTION_H

#include <QAction>

class AbstractImageProcessorWithDialogOption;

/**
 * Class of action to wrap image processor with simple option.
 */
class ImageProcessorWithComplexOptionAction : public QAction
{
public:
  /**
   * Constructor.
   *
   * @param configurationInstance One instance of the processor to set
   * configuration.
   */
  ImageProcessorWithComplexOptionAction(AbstractImageProcessorWithDialogOption *
                                        configurationInstance,
                                        QObject *parent);

  /**
   * @return The instance of the processor to set configuration.
   */
  inline AbstractImageProcessorWithDialogOption * getConfiguarionInstance()
  {
    return _configurationInstance;
  }

private:
  /**
   * The one instance of the processor to set configuration.
   */
  AbstractImageProcessorWithDialogOption *_configurationInstance;
};

#endif // IMAGEPROCESSORWITHCOMPLEXOPTIONACTION_H
