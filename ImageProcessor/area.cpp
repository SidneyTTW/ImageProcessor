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

void Area::paint(QPainter *painter, int width, int height)
{
  QPainter *backup = painter;
  QPainter *newPainter = NULL;
  QImage *newImage = NULL;
  if (conversed)
  {
    newImage = new QImage(width, height, QImage::Format_ARGB32);
    newImage->fill(qRgba(255, 255, 255, 0));
    newPainter = new QPainter(newImage);
    newPainter->setPen(painter->pen());
    newPainter->setBrush(painter->brush());
    painter = newPainter;
  }
  switch (mainType)
  {
  case TypeRectangle:
    painter->drawRect(_rectangle);
    break;
  case TypePolygon:
    painter->drawPolygon(_polygon);
    break;
  case TypeEllipse:
    painter->drawEllipse(_ellipse.getCenter(),
                         _ellipse.getDx(),
                         _ellipse.getDy());
    break;
  default:
    break;
  }
  if (conversed)
  {
    newPainter->setCompositionMode(QPainter::CompositionMode_DestinationOut);
    newPainter->drawRect(0, 0, width, height);
    newPainter->end();
    delete newPainter;
    backup->drawImage(0, 0, *newImage);
    delete newImage;
  }
}
