#ifndef ABSTRACTCOLORCHOOSER_H
#define ABSTRACTCOLORCHOOSER_H

#include <QColor>

/**
 * Abstract class to get a color.
 */
class AbstractColorChooser
{
public:
  virtual QColor getCurrentColor() const=0;
  virtual QColor getBackgroundColor() const=0;
};

#endif // ABSTRACTCOLORCHOOSER_H
