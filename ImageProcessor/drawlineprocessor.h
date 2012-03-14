#ifndef DRAWLINEPROCESSOR_H
#define DRAWLINEPROCESSOR_H

#include "abstractimageprocessorwithsimpleoption.h"

#include <QSpinBox>

/**
 * Class of processor to draw a line.
 */
class DrawLineProcessor : public AbstractImageProcessorWithSimpleOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  DrawLineProcessor();

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
  QPoint startPosition;

  /**
   * End position of the line.
   */
  QPoint endPosition;

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

#endif // DRAWLINEPROCESSOR_H
