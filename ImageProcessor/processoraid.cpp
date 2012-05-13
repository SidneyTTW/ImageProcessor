#include "processoraid.h"

#include <QFile>
#include <QTextStream>
#include "algebraoperationprocessor.h"
#include "boundprocessor.h"
#include "dilationerosionprocessor.h"
#include "drawlineprocessor.h"
#include "eraseprocessor.h"
#include "fastgaussprocessor.h"
#include "filterprocessor.h"
#include "histogramequalizationprocessor.h"
#include "histogrammatchprocessor.h"
#include "hslprocessor.h"
#include "magiceraseprocessor.h"
#include "penprocessor.h"
#include "pointprocessor.h"
#include "redeyereductionprocessor.h"
#include "resizeprocessor.h"
#include "reverseprocessor.h"
#include "rgbmapprocessor.h"
#include "rotateprocessor.h"
#include "sharpenprocessor.h"
#include "tograyprocessor.h"
#include "toblackandwhiteprocessor.h"

QVector<AbstractImageProcessorWithSimpleOption *> ProcessorAid::simpleOptions()
{
  QVector<AbstractImageProcessorWithSimpleOption *> result;
  result.push_back(new DrawLineProcessor());
  result.push_back(new PenProcessor());
  result.push_back(new ToGrayProcessor());
  result.push_back(new ReverseProcessor());
  result.push_back(new HistogramEqualizationProcessor());
  result.push_back(new FastGaussProcessor());
  result.push_back(new SharpenProcessor());
  result.push_back(new RedEyeReductionProcessor());
  result.push_back(new EraseProcessor());
  result.push_back(new MagicEraseProcessor());
  return result;
}

QVector<AbstractImageProcessorWithDialogOption *> ProcessorAid::complexOptions()
{
  QVector<AbstractImageProcessorWithDialogOption *> result;
  result.push_back(new ToBlackAndWhiteProcessor());
  result.push_back(new ResizeProcessor());
  result.push_back(new RotateProcessor());
  result.push_back(new FilterProcessor());
  result.push_back(new BoundProcessor());
  result.push_back(new HSLProcessor());
  result.push_back(new AlgebraOperationProcessor());
  result.push_back(new HistogramMatchProcessor());
  result.push_back(new PointProcessor());
  result.push_back(new DilationErosionProcessor());
  result.push_back(new RGBMapProcessor());
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
