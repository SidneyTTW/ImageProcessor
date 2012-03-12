#include "imageprocessorwithsimpleoptionaction.h"

ImageProcessorWithSimpleOptionAction::ImageProcessorWithSimpleOptionAction
    (AbstractImageProcessorWithSimpleOption *configurationInstance,
     QObject *parent) :
    QAction("", parent),
    _configurationInstance(configurationInstance)
{
}
