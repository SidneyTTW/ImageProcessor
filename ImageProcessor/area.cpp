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
  QPainter::CompositionMode mode = painter->compositionMode();
  painter->setCompositionMode(QPainter::CompositionMode_Xor);
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
  painter->setCompositionMode(mode);
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

QRect Area::bound() const
{
  switch (mainType)
  {
  case TypeRectangle:
    return _rectangle;
    break;
  case TypePolygon:
    return _polygon.boundingRect();
    break;
  case TypeEllipse:
    return QRect(_ellipse.getCenter().x() - _ellipse.getDx(),
                 _ellipse.getCenter().y() - _ellipse.getDy(),
                 _ellipse.getDx() * 2 + 2,
                 _ellipse.getDy() * 2 + 2);
    break;
  default:
    return QRect();
    break;
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
    {
      int values[4];
      values[0] = list.takeFirst().toInt();
      values[1] = list.takeFirst().toInt();
      values[2] = list.takeFirst().toInt();
      values[3] = list.takeFirst().toInt();
      result = Area(QRect(values[0],
                          values[1],
                          values[2],
                          values[3]));
    }
    break;
  case TypePolygon:
    {
      int size = list.takeFirst().toInt();
      if (list.size() != 2 * size)
        break;
      QPolygon poly;
      for (int i = 0;i < size;++i)
      {
        int values[2];
        values[0] = list.takeFirst().toInt();
        values[1] = list.takeFirst().toInt();
        poly.append(QPoint(values[0], values[1]));
      }
      result = Area(poly);
    }
    break;
  case TypeEllipse:
    if (list.size() != 4)
      break;
    {
      int values[4];
      values[0] = list.takeFirst().toInt();
      values[1] = list.takeFirst().toInt();
      values[2] = list.takeFirst().toInt();
      values[3] = list.takeFirst().toInt();
      result = Area(Ellipse(QPoint(values[0],
                                   values[1]),
                            values[2],
                            values[3]));
    }
    break;
  default:
    break;
  }
  return result;
}
