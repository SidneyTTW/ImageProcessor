#ifndef AREA_H
#define AREA_H

#include <QPainter>
#include <QPolygon>
#include <QRect>
#include "ellipse.h"

/**
 * Class to describe an area.
 * The area can be empty, rectangle, complex polygon or ellipse.
 * The area can also find whether the area is square or circle.
 * Furthermore, the area can be conversed.
 */
class Area
{
public:
  /**
   * Type flag of the area.
   */
  enum AreaTypeFlag {TypeRectangle = 1,
                     TypeSquare = 3,
                     TypePolygon = 4,
                     TypeEllipse = 8,
                     TypeCircle = 24,
                     TypeEmpty = 256};

  /**
   * The type of the area.
   */
  typedef QFlags<AreaTypeFlag> AreaType;

  /**
   * Constructor which will make an empty area.
   */
  Area();

  /**
   * Constructor with polygon.
   *
   * @param polygon The polygon.
   */
  Area(const QPolygon& polygon);

  /**
   * Constructor with rectangle.
   *
   * @param rectangle The rectangle.
   */
  Area(const QRect& rectangle);

  /**
   * Constructor with ellipse.
   *
   * @param ellipse The ellipse.
   */
  Area(const Ellipse& ellipse);

  /**
   * Troggle whether the area is conversed.
   */
  inline void troggle() {
    conversed = !conversed;
  }

  /**
   * @return Whether the area is conversed.
   */
  inline bool isConversed() {
    return conversed;
  }

  /**
   * @return Type the type.
   */
  AreaTypeFlag getType();

  /**
   * Paint the area.
   *
   * @param painter The painter.
   * @param brush The brush.
   * @param width The width of the area.
   * @param height The height of the area.
   */
  void paint(QPainter *painter, int width, int height);

private:
  /**
   * The main type of the area, should be one of Rectangle, Polygon, Ellipse, Empty.
   */
  AreaTypeFlag mainType;

  /**
   * Whether the area is conversed.
   */
  bool conversed;

  /**
   * The polygon.
   */
  QPolygon _polygon;

  /**
   * The rectangle.
   */
  QRect _rectangle;

  /**
   * The ellipse.
   */
  Ellipse _ellipse;
};

#endif // AREA_H
