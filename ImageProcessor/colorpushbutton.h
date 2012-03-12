#ifndef COLORPUSHBUTTON_H
#define COLORPUSHBUTTON_H

#include <QPushButton>
#include <QColor>

/**
 * Class of plain push button with single color.
 */
class ColorPushButton : public QPushButton
{
public:
  /**
   * Constructor.
   */
  ColorPushButton(QColor color);

  /**
   * @param The color to set.
   */
  inline void setColor(QColor color)
  {
    _color = color;
    update();
  }

  /**
   * @return The color.
   */
  inline QColor getColor()
  {
    return _color;
  }

protected:
  virtual void paintEvent(QPaintEvent *event);
  virtual void mouseDoubleClickEvent(QMouseEvent *event);

private:
  /**
   * The color.
   */
  QColor _color;
};

#endif // COLORPUSHBUTTON_H
