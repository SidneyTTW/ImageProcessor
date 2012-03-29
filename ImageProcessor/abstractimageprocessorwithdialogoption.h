#ifndef ABSTRACTIMAGEPROCESSORWITHDIALOGOPTION_H
#define ABSTRACTIMAGEPROCESSORWITHDIALOGOPTION_H

#include "abstractimageprocessor.h"
#include "area.h"

/**
 * Abstract class of image processor which use a dialog to set option.
 * In consideration that a complex process may deal with only part of the full
 * image, the processor should accept an area as input and return the result
 * area.
 */
class AbstractImageProcessorWithDialogOption : public AbstractImageProcessor
{
public:
  /**
   * @return The types of input it can accept.
   */
  virtual Area::AreaType acceptableAreaType() const=0;

  /**
   * @return The types of result.
   */
  virtual Area::AreaTypeFlag resultAreaType() const=0;

  /**
   * @param type The type.
   *
   * @return Whether the type can be accepted.
   */
  bool inline canAcceptArea(Area::AreaTypeFlag type) const
  {
    Area::AreaType acceptable = acceptableAreaType();
    return acceptable.testFlag(type);
  }

  /**
   * @return The result area.
   */
  virtual Area resultArea() const=0;

  /**
   * @return The dialog to choose option.
   */
  virtual QDialog *getOptionDialog(Area area, const MyImage& image)=0;
};

#endif // ABSTRACTIMAGEPROCESSORWITHDIALOGOPTION_H
