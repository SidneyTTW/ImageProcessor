#ifndef MIDNUMBERPROCESSOR_H
#define MIDNUMBERPROCESSOR_H

#include "abstractimageprocessorwithsimpleoption.h"

/**
 * Class of processor to process an image with mid number.
 */
class MidNumberProcessor : public AbstractImageProcessorWithSimpleOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  MidNumberProcessor();

  virtual MyImage::ImageTypeFlag resultType() const;
  virtual QImage *processImage(const QImage& image) const;
  virtual void processImage(QImage *image) const;
  virtual QWidget *optionWidget();
  virtual MyImage preProcessImage(const MyImage& image) const;
  virtual bool cancelWhenNewOneIsCreated() const;
  virtual void interrupt();
  virtual QString name() const;
  virtual QString toString() const;
  virtual AbstractImageProcessor *fromString(const QString& str) const;

private:
  /**
   * Width of the matrix.
   */
  int _width;

  /**
   * Height of the matrix.
   */
  int _height;

  /**
   * The option widget.
   */
  QWidget *_optionWidget;

private slots:
  /**
   * Slot to change width.
   */
  void changeWidth(int width);

  /**
   * Slot to change height.
   */
  void changeHeight(int height);

  /**
   * Slot called when the option has been confirmed.
   */
  void confirm();
};

#endif // MIDNUMBERPROCESSOR_H
