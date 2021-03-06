#include "ellipse.h"

Ellipse::Ellipse() :
    _center(QPoint(0, 0)),
    _dx(0),
    _dy(0)
{
}

Ellipse::Ellipse(const QPoint& center, int dx, int dy) :
    _center(center),
    _dx(dx),
    _dy(dy)
{
}

bool Ellipse::isIn(const QPoint& point) const
{
  int fakeR = qMax(_dx, _dy);

  int dx = point.x() - _center.x();
  int dy = point.y() - _center.y();

  double fakeDx = 1.0 * dx * fakeR / _dx;
  double fakeDy = 1.0 * dy * fakeR / _dy;

  return fakeDx * fakeDx + fakeDy * fakeDy <= fakeR * fakeR;
}
