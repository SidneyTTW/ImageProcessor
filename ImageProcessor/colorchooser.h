#ifndef COLORCHOOSER_H
#define COLORCHOOSER_H

#include <QWidget>
#include "abstractcolorchooser.h"

#include <QGridLayout>

class ColorPushButton;

/**
 * Class of widget to choose color.
 */
class ColorChooser : public QWidget, public AbstractColorChooser
{
  Q_OBJECT

public:
  static const int COLOR_COUNT = 28;

  /**
   * Constructor.
   */
  explicit ColorChooser(QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~ColorChooser();

  virtual QColor getCurrentColor() const;
  virtual QColor getBackgroundColor() const;

protected:
  virtual bool eventFilter(QObject *object, QEvent *event);

private:
  /**
   * The buttons to select colors.
   */
  ColorPushButton *colorButtons[COLOR_COUNT];

  /**
   * The buttons to show the fore color.
   */
  ColorPushButton *foreButton;

  /**
   * The buttons to show the back color.
   */
  ColorPushButton *backButton;

  /**
   * The layout.
   */
  QGridLayout *gridLayout;
};

#endif // COLORCHOOSER_H
