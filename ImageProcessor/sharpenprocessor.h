#ifndef SHARPENPROCESSOR_H
#define SHARPENPROCESSOR_H

#include "abstractimageprocessorwithsimpleoption.h"

#include "imagealgorithm.h"
#include <QSpinBox>

/**
 * Class of processor to use gauss operator immidiately.
 */
class SharpenProcessor : public AbstractImageProcessorWithSimpleOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  SharpenProcessor();

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
   * Width of the line.
   */
  int _radius;

  /**
   * Start position of the line.
   */
  QPolygon positions;

  /**
   * The option widget.
   */
  QSpinBox *_optionWidget;

  /**
   * The convolution core.
   */
  static int _convolutionCore[9];

public slots:
  /**
   * Slot used to change the radius.
   */
  void changeToRadius(int radius);
};

#endif // SHARPENPROCESSOR_H
