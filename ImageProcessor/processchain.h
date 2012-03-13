#ifndef PROCESSCHAIN_H
#define PROCESSCHAIN_H


#include <QList>
#include "abstractimageprocessor.h"
#include "myimage.h"

/**
 * Class to store chain of processor and the images before/after the process.
 */
class ProcessChain
{
public:
  static const int DEFAULT_CHAIN_LIMIT = 0;

  /**
   * Constructor.
   *
   * @param image First image.
   * @param limit The limit of the number of processor recorded.
   */
  ProcessChain(MyImage *image, int limit=DEFAULT_CHAIN_LIMIT);

  /**
   * Destructor.
   */
  ~ProcessChain();

  /**
   * Add a processor at last position.
   *
   * @param processor The processor.
   */
  void addProcessor(AbstractImageProcessor *processor);

  /**
   * Add a processor at current position.
   *
   * @param processor The processor.
   */
  void addProcessorAtCurrentPosition(AbstractImageProcessor *processor);

  /**
   * Add a processor at given index.
   * The original processor/image at/after the given index will be deleted.
   *
   * @param processor The processor.
   * @param index The index.
   */
  void addProcessorAt(AbstractImageProcessor *processor, int index);

  /**
   * @return The limit of the number of processor recorded. 0 means unlimited.
   */
  inline int getLimit()
  {
    return _limit;
  }

  /**
   * Set the limit of the number of processor recorded. 0 means unlimited.
   *
   * @param limit The new limit.
   */
  void setLimit(int limit);

  /**
   * @param i The index of the image.
   * @return The image at given position.
   */
  inline MyImage *getImageAt(int i)
  {
    if (i >= 0 && i < imageList.size())
      return imageList.at(i);
    if (i == 0)
      return firstImage;
    return NULL;
  }

  /**
   * @return The curent image.
   */
  inline MyImage *getCurrentImage()
  {
    if (_currentIndex >= 0 && _currentIndex < imageList.size())
      return imageList.at(_currentIndex);
    return firstImage;
  }

  /**
   * @return The curent image type.
   */
  inline MyImage::ImageTypeFlag getCurrentType()
  {
    return getCurrentImage()->getType();
  }

  /**
   * @return The last image.
   */
  inline MyImage *getLastImage()
  {
    if (!imageList.isEmpty())
      return imageList.last();
    return firstImage;
  }

  /**
   * Move to the previous image.
   */
  inline void previous()
  {
    if (_currentIndex >= 0)
      --_currentIndex;
  }

  /**
   * Move to the next image.
   */
  inline void next()
  {
    if (_currentIndex < processorList.size() - 1)
      ++_currentIndex;
  }

  /**
   * Compress the space.
   */
  void compress();

  /**
   * Save the chain.
   *
   * @param path The path of the file.
   */
  void save(QString path);

private:
  /**
   * The current.
   */
  int _currentIndex;

  /**
   * The limit of the number of processor recorded. 0 means unlimited.
   */
  int _limit;

  /**
   * The first image.
   */
  MyImage *firstImage;

  /**
   * The processor list.
   */
  QList<AbstractImageProcessor *> processorList;

  /**
   * The recycled processor list.
   */
  QList<AbstractImageProcessor *> recycledProcessorList;

  /**
   * The image list.
   */
  QList<MyImage *> imageList;

  /**
   * Maintain the lists according to the limit.
   */
  void maintainLists();
};

#endif // PROCESSCHAIN_H
