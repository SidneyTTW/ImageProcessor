#include "processchain.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>
#include "abstractimageprocessor.h"
#include "processoraid.h"

ProcessChain::ProcessChain(MyImage *image, int limit) :
    firstImage(image),
    _limit(limit)
{
  _currentIndex = -1;
}

void ProcessChain::addProcessor(AbstractImageProcessor *processor)
{
  MyImage *lastImage = processorList.isEmpty() ? firstImage : imageList.last();
  processorList.append(processor);
  imageList.append(processor->processImage(*lastImage));
  _currentIndex = imageList.size() - 1;
  maintainLists();
}

void ProcessChain::addProcessorAtCurrentPosition(AbstractImageProcessor *processor)
{
  addProcessorAt(processor, _currentIndex + 1);
}

void ProcessChain::addProcessorAt(AbstractImageProcessor *processor, int index)
{
  if (index >= 0 && index <= processorList.size())
  {
    int remainCount = 0;
    // Find the first thing to delete
    QList<AbstractImageProcessor *>::Iterator itr1 = processorList.begin();
    QList<MyImage *>::Iterator itr2 = imageList.begin();
    for (;remainCount < index;++remainCount, ++itr1, ++itr2);
    // Delete the things
    while (itr1 != processorList.end())
    {
      delete *itr1;
      delete *itr2;
      itr1 = processorList.erase(itr1);
      itr2 = imageList.erase(itr2);
    }
    addProcessor(processor);
  }
  else
    addProcessor(processor);
}

void ProcessChain::maintainLists()
{
  _currentIndex = imageList.size() - 1;
  if (_limit == 0 || processorList.size() <= _limit)
    return;
  QList<AbstractImageProcessor *>::Iterator itr1 = processorList.begin();
  QList<MyImage *>::Iterator itr2 = imageList.begin();
  while (processorList.size() > _limit)
  {
    recycledProcessorList.append(*itr1);
    delete *itr2;
    itr1 = processorList.erase(itr1);
    itr2 = imageList.erase(itr2);
  }
  if (!processorList.isEmpty())
  {
    recycledProcessorList.append(processorList.takeFirst());
    processorList.insert(0, NULL);
    _currentIndex = imageList.size() - 1;
  }
}

void ProcessChain::compress()
{
  QList<AbstractImageProcessor *>::Iterator itr1 = processorList.begin();
  QList<MyImage *>::Iterator itr2 = imageList.begin();
  while (processorList.size() > 1)
  {
    recycledProcessorList.append(*itr1);
    delete *itr2;
    itr1 = processorList.erase(itr1);
    itr2 = imageList.erase(itr2);
  }
  if (!processorList.isEmpty())
  {
    recycledProcessorList.append(processorList.takeFirst());
    processorList.insert(0, NULL);
    _currentIndex = 0;
  }
}

void ProcessChain::setLimit(int limit)
{
  _limit = limit;
  maintainLists();
}

void ProcessChain::save(QString path)
{
  QFile file(path);
  if (file.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream out(&file);
    AbstractImageProcessor *processor;
    foreach (processor, recycledProcessorList)
      out << processor->name() << '|' << processor->toString() << '|';
    foreach (processor, processorList)
      if (processor != NULL)
        out << processor->name() << '|' << processor->toString() << '|';
  }
  file.close();
}

QList<AbstractImageProcessor *> ProcessChain::loadProcessor(QString path)
{
  QList<AbstractImageProcessor *> result;
  QFile file(path);
  if (file.open(QFile::ReadOnly)) {
    QMap<QString, AbstractImageProcessor *> all = ProcessorAid::allProcessor();
    QTextStream in(&file);
    QString str = in.readAll();
    QStringList list = str.split('|', QString::SkipEmptyParts);
    while (list.size() >= 2)
    {
      QString name = list.takeFirst();
      QString detail = list.takeFirst();
      AbstractImageProcessor *processor = all.value(name, NULL);
      if (processor != NULL)
        result.push_back(processor->fromString(detail));
    }
    for (QMap<QString, AbstractImageProcessor *>::Iterator itr = all.begin();
         itr != all.end();
         ++itr)
      delete itr.value();
  }
  return result;
}

ProcessChain::~ProcessChain()
{
  delete firstImage;
  AbstractImageProcessor *processor;
  foreach (processor, processorList)
    delete processor;
  foreach (processor, recycledProcessorList)
    delete processor;
  MyImage *image;
  foreach (image, imageList)
    delete image;
}
