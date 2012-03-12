#include "processchain.h"

#include "abstractimageprocessor.h"

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
    delete *itr1;
    delete *itr2;
    itr1 = processorList.erase(itr1);
    itr2 = imageList.erase(itr2);
  }
  if (!processorList.isEmpty())
  {
    delete processorList.first();
    processorList.removeAt(0);
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
    delete *itr1;
    delete *itr2;
    itr1 = processorList.erase(itr1);
    itr2 = imageList.erase(itr2);
  }
  if (!processorList.isEmpty())
  {
    delete processorList.first();
    processorList.removeAt(0);
    processorList.insert(0, NULL);
    _currentIndex = 0;
  }
}

void ProcessChain::setLimit(int limit)
{
  _limit = limit;
  maintainLists();
}

ProcessChain::~ProcessChain()
{
  delete firstImage;
  AbstractImageProcessor *processor;
  foreach (processor, processorList)
    delete processor;
  MyImage *image;
  foreach (image, imageList)
    delete image;
}
