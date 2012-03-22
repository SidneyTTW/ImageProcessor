#include "processoraid.h"

#include <QFile>
#include <QTextStream>
#include "convolutionprocessor.h"
#include "drawlineprocessor.h"
#include "midnumberprocessor.h"
#include "histogramequalizationprocessor.h"
#include "penprocessor.h"
#include "resizeprocessor.h"
#include "reverseprocessor.h"
#include "tograyprocessor.h"
#include "toblackandwhiteprocessor.h"

QVector<AbstractImageProcessorWithSimpleOption *> ProcessorAid::simpleOptions()
{
  QVector<AbstractImageProcessorWithSimpleOption *> result;
  result.push_back(new DrawLineProcessor());
  result.push_back(new PenProcessor());
  result.push_back(new ToGrayProcessor());
  result.push_back(new MidNumberProcessor());
  result.push_back(new ReverseProcessor());
  result.push_back(new HistogramEqualizationProcessor());
  return result;
}

QVector<AbstractImageProcessorWithDialogOption *> ProcessorAid::complexOptions()
{
  QVector<AbstractImageProcessorWithDialogOption *> result;
  result.push_back(new ToBlackAndWhiteProcessor());
  result.push_back(new ConvolutionProcessor());
  result.push_back(new ResizeProcessor());
  return result;
}

QMap<QString, AbstractImageProcessor *> ProcessorAid::allProcessor()
{
  QVector<AbstractImageProcessorWithSimpleOption *> simple = simpleOptions();
  QVector<AbstractImageProcessorWithDialogOption *> complex = complexOptions();
  QMap<QString, AbstractImageProcessor *> result;
  AbstractImageProcessor *processor;
  foreach (processor, simple)
    result.insert(processor->name(), processor);
  foreach (processor, complex)
    result.insert(processor->name(), processor);
  return result;
}
