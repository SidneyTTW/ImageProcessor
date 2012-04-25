#ifndef ERASEPROCESSOR_H
#define ERASEPROCESSOR_H

#include "abstractimageprocessorwithsimpleoption.h"

#include "imagealgorithm.h"
#include <QSpinBox>

/**
 * Class of processor to erase an area.
 */
class EraseProcessor : public AbstractImageProcessorWithSimpleOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  EraseProcessor();

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
   * Size of the processor.
   */
  int _size;

  /**
   * Center positions to erase.
   */
  QPolygon positions;

  /**
   * Color of the background.
   */
  QColor _color;

  /**
   * The option widget.
   */
  QSpinBox *_optionWidget;

public slots:
  /**
   * Slot used to change the radius.
   */
  void changeToSize(int size);
};
#endif // ERASEPROCESSOR_H
