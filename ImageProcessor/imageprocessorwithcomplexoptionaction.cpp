#include "imageprocessorwithcomplexoptionaction.h"

ImageProcessorWithComplexOptionAction::ImageProcessorWithComplexOptionAction
    (AbstractImageProcessorWithDialogOption *configurationInstance,
     QObject *parent) :
    QAction("", parent),
    _configurationInstance(configurationInstance)
{
}
