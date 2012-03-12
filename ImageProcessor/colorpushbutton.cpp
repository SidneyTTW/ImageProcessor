#include "colorpushbutton.h"

#include <QPainter>

ColorPushButton::ColorPushButton(QColor color) :
    _color(color)
{
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFlat(true);
}

void ColorPushButton::paintEvent(QPaintEvent *event)
{
  QPainter *painter = new QPainter(this);
  painter->fillRect(0, 0, width(), height(), _color);
  painter->end();
  delete painter;
}
