#ifndef MAGICERASEPROCESSOR_H
#define MAGICERASEPROCESSOR_H

#include "abstractimageprocessorwithsimpleoption.h"

#include <QSpinBox>

/**
 * Class of processor to do magic erase.
 */
class MagicEraseProcessor : public AbstractImageProcessorWithSimpleOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  MagicEraseProcessor();

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
  virtual QString description() const;
  virtual QString iconPath() const;

protected:
  /**
   * Used to filter the events.
   */
  virtual bool eventFilter(QObject *object, QEvent *event);

private:
  /**
   * Whether the processor is valid.
   */
  bool valid;

  /**
   * Allowed difference.
   */
  int _allowedDifference;

  /**
   * Position to start to erase.
   */
  QPoint pos;

  /**
   * The option widget.
   */
  QSpinBox *_optionWidget;

public slots:
  /**
   * Slot used to change the allowed difference.
   */
  void changeToAllowedDifference(int allowedDifference);
};

#endif // MAGICERASEPROCESSOR_H
