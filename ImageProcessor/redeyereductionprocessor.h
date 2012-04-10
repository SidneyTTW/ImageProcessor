#ifndef REDEYEREDUCTIONPROCESSOR_H
#define REDEYEREDUCTIONPROCESSOR_H

#include "abstractimageprocessorwithsimpleoption.h"

#include "imagealgorithm.h"
#include <QSpinBox>

/**
 * Class of processor to use gauss operator immidiately.
 */
class RedEyeReductionProcessor : public AbstractImageProcessorWithSimpleOption
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  RedEyeReductionProcessor();

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

public slots:
  /**
   * Slot used to change the radius.
   */
  void changeToRadius(int radius);
};

#endif // REDEYEREDUCTIONPROCESSOR_H
