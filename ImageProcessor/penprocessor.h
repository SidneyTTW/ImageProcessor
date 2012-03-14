#ifndef PENPROCESSOR_H
#define PENPROCESSOR_H

#include "abstractimageprocessorwithsimpleoption.h"

#include "imagealgorithm.h"
#include <QSpinBox>

/**
 * Class of processor to convert an image into gray one.
 */
class PenProcessor : public AbstractImageProcessorWithSimpleOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  PenProcessor();

  virtual MyImage::ImageTypeFlag resultType() const;
  virtual QImage *processImage(const QImage& image) const;
  virtual QWidget *optionWidget();
  virtual MyImage preProcessImage(const MyImage& image) const;
  virtual bool cancelWhenNewOneIsCreated() const;
  virtual void interrupt();
  virtual QString name() const;
  virtual QString toString() const;
  virtual AbstractImageProcessor *fromString(const QString& str) const;

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
   * Width of the line.
   */
  int _width;

  /**
   * Color of the line.
   */
  QColor _color;

  /**
   * Start position of the line.
   */
  QPolygon positions;

  /**
   * The option widget.
   */
  QSpinBox *_optionWidget;

public slots:
  /**
   * Slot used to change the width.
   */
  void changeToWidth(int width);
};

#endif // PENPROCESSOR_H
