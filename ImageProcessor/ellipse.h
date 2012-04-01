#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QPoint>

/**
 * Class to describe an ellipse with the center and dx, dy.
 */
class Ellipse
{
public:
  /**
   * Default constructor.
   */
  Ellipse();

  /**
   * Constructor with the center and dx, dy.
   *
   * @param center The center.
   * @param dx The dx.
   * @param dy The dy.
   */
  Ellipse(const QPoint& center, int dx, int dy);

  /**
   * Calculate whether a point is in the ellipse.
   *
   * @param The point.
   * @return Whether the point is in the ellipse.
   */
  bool isIn(const QPoint& point);

  /**
   * @return The center.
   */
  inline QPoint getCenter() const
  {
    return _center;
  }

  /**
   * @return The dx.
   */
  inline int getDx() const
  {
    return _dx;
  }

  /**
   * @return The dy.
   */
  inline int getDy() const
  {
    return _dy;
  }

  /**
   * @return Whether it's a circle.
   */
  inline bool isCircle() {
    return _dx == _dy;
  }

private:
  /**
   * The center.
   */
  QPoint _center;

  /**
   * The dx.
   */
  int _dx;

  /**
   * The dy.
   */
  int _dy;
};

#endif // ELLIPSE_H
