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

Area::AreaTypeFlag Area::getType() const
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

QString Area::toString() const
{
  QString result = QObject::tr("%1").arg((int)mainType);
  switch (mainType)
  {
  case TypeRectangle:
    result += QObject::tr(",%1,%2,%3,%4").
              arg(_rectangle.left()).
              arg(_rectangle.top()).
              arg(_rectangle.width()).
              arg(_rectangle.height());
    break;
  case TypePolygon:
    result += QObject::tr(",%1").arg(_polygon.size());
    for (int i = 0;i < _polygon.size();++i)
      result += QObject::tr(",%1,%2").
                arg(_polygon[i].x()).
                arg(_polygon[i].y());
    break;
  case TypeEllipse:
    result += QObject::tr(",%1,%2,%3,%4").
              arg(_ellipse.getCenter().x()).
              arg(_ellipse.getCenter().y()).
              arg(_ellipse.getDx()).
              arg(_ellipse.getDy());
    break;
  default:
    break;
  }
  return result;
}

Area Area::fromString(const QString& str)
{
  Area result;
  QStringList list = str.split(',', QString::SkipEmptyParts);
  if (list.isEmpty())
    return result;
  AreaTypeFlag type = (AreaTypeFlag) list.takeFirst().toInt();
  switch (type)
  {
  case TypeRectangle:
    if (list.size() != 4)
      break;
    result = Area(QRect(list.takeFirst().toInt(),
                        list.takeFirst().toInt(),
                        list.takeFirst().toInt(),
                        list.takeFirst().toInt()));
    break;
  case TypePolygon:
    {
      int size = list.takeFirst().toInt();
      if (list.size() != 2 * size)
        break;
      QPolygon poly;
      for (int i = 0;i < size;++i)
        poly.append(QPoint(list.takeFirst().toInt(),
                           list.takeFirst().toInt()));
      result = Area(poly);
    }
    break;
  case TypeEllipse:
    if (list.size() != 4)
      break;
    result = Area(Ellipse(QPoint(list.takeFirst().toInt(),
                                 list.takeFirst().toInt()),
                          list.takeFirst().toInt(),
                          list.takeFirst().toInt()));
    break;
  default:
    break;
  }
  return result;
}
