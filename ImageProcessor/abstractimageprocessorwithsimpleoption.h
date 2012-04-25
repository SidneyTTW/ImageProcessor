#ifndef ABSTRACTIMAGEPROCESSORWITHSIMPLEOPTION_H
#define ABSTRACTIMAGEPROCESSORWITHSIMPLEOPTION_H

#include "abstractcolorchooser.h"
#include "abstractimageprocessor.h"

/**
 * Abstract class of image processor which use a small widget to set option.
 * Should be able to accept image of any type.
 * Should override eventFilter.
 */
class AbstractImageProcessorWithSimpleOption : public AbstractImageProcessor
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  AbstractImageProcessorWithSimpleOption() : _colorChooser(NULL) {}

  /**
   * @return A widget connected with the configure instance to set the
   * confuration of the processor.
   */
  virtual QWidget *optionWidget()=0;

  /**
   * @return A preprocessed image according to the configure instance.
   */
  virtual MyImage preProcessImage(const MyImage& image) const=0;

  /**
   * @return Whether to cancel the processor when new one is created.
   */
  virtual bool cancelWhenNewOneIsCreated() const=0;

  /**
   * Interrupt the configure instance.
   */
  virtual void interrupt()=0;

  virtual MyImage::ImageType acceptableType() const
  {
    return (MyImage::ImageType)(MyImage::Colored | MyImage::Gray | MyImage::BlackAndWhite);
  }

  /**
   * Set color chooser.
   *
   * @param n The current color of the color chooser.
   */
  inline void setColorChooser(AbstractColorChooser *colorChooser)
  {
    _colorChooser = colorChooser;
  }

  /**
   * @return The current color of the color chooser.
   */
  inline QColor getCurrentColor() const
  {
    if (_colorChooser != NULL)
      return _colorChooser->getCurrentColor();
    return QColor();
  }

  /**
   * @return The current color of the color chooser.
   */
  inline QColor getBackgroundColor() const
  {
    if (_colorChooser != NULL)
      return _colorChooser->getBackgroundColor();
    return QColor();
  }

protected:
  /**
   * The color chooser.
   */
  AbstractColorChooser *_colorChooser;

signals:
  /**
   * Signal emitted when the option is changed.
   */
  void optionChanged(AbstractImageProcessorWithSimpleOption *processor);
};

#endif // ABSTRACTIMAGEPROCESSORWITHSIMPLEOPTION_H
