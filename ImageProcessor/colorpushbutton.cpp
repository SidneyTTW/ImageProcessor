#include "colorpushbutton.h"

#include <QColorDialog>
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

void ColorPushButton::mouseDoubleClickEvent(QMouseEvent *event)
{
  QColor color = QColorDialog::getColor(_color);
  if (color.isValid())
    setColor(color);
}
