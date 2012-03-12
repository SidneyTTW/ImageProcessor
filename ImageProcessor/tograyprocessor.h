#ifndef TOGRAYPROCESSOR_H
#define TOGRAYPROCESSOR_H

#include "abstractimageprocessorwithsimpleoption.h"

#include "imagealgorithm.h"

class ToGrayOptionWidget;

/**
 * Class of processor to convert an image into gray one.
 */
class ToGrayProcessor : public AbstractImageProcessorWithSimpleOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  ToGrayProcessor();

  virtual MyImage::ImageTypeFlag resultType() const;
  virtual MyImage *processImage(const MyImage& image) const;
  virtual QWidget *optionWidget();
  virtual MyImage preProcessImage(const MyImage& image) const;
  virtual bool cancelWhenNewOneIsCreated() const;
  virtual void interrupt();
  virtual QString name() const;

  /**
   * @return The type of the algorithm.
   */
  inline ImageAlgorithm::ImageToGrayAlgorithmType getType() const
  {
    return _type;
  }

private:
  /**
   * The type of the algorithm.
   */
  ImageAlgorithm::ImageToGrayAlgorithmType _type;

  /**
   * The option widget.
   */
  ToGrayOptionWidget *_optionWidget;

private slots:
  /**
   * Slot called when the option has been confirmed.
   */
  void confirm();

public slots:
  /**
   * Slot used to change the type.
   */
  void changeToType(ImageAlgorithm::ImageToGrayAlgorithmType type);
};

#endif // TOGRAYPROCESSOR_H
