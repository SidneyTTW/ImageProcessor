#ifndef REVERSEPROCESSOR_H
#define REVERSEPROCESSOR_H

#include "abstractimageprocessorwithsimpleoption.h"

#include <QPushButton>

/**
 * Class of processor to convert an image into gray one.
 */
class ReverseProcessor : public AbstractImageProcessorWithSimpleOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  ReverseProcessor();

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
   * The option widget.
   */
  QPushButton *_optionWidget;

private slots:
  /**
   * Slot called when the option has been confirmed.
   */
  void confirm();
};

#endif // REVERSEPROCESSOR_H
