#include "area.h"

Area::Area() : mainType(TypeEmpty)
{
}

Area::Area(const QPolygon& polygon) :
    _polygon(polygon),
    mainType(TypePolygon)
{
}

Area::Area(const QRect& rectangle) :
    _rectangle(rectangle),
    mainType(TypeRectangle)
{
}

Area::Area(const Ellipse& ellipse) :
    _ellipse(ellipse),
    mainType(TypeEllipse)
{
}

Area::AreaTypeFlag Area::getType()
{
  switch (mainType)
  {
  case TypePolygon:
  case TypeEmpty:
    return mainType;
  case TypeRectangle:
    return _rectangle.width() == _rectangle.height() ? TypeSquare: TypeRectangle;
  case TypeEllipse:
    return _ellipse.isCircle() ? TypeCircle : TypeEllipse;
  default:
    return TypeEmpty;
  }
}
