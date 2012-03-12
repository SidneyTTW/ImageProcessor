#ifndef SIMPLEOPTIONCONTAINERWIDGET_H
#define SIMPLEOPTIONCONTAINERWIDGET_H

#include <QWidget>
#include <QStackedLayout>

/**
 * A widget used to store a simple option widget.
 */
class SimpleOptionContainerWidget : public QWidget
{
  Q_OBJECT
public:
  /**
   * Default constructor.
   */
  explicit SimpleOptionContainerWidget(QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~SimpleOptionContainerWidget();

  /**
   * Set the widget to show.
   */
  void setWidget(QWidget *widget);

  /**
   * Clear the widgets.
   */
  void clear();

private:
  /**
   * The layout.
   */
  QStackedLayout *layout;
};

#endif // SIMPLEOPTIONCONTAINERWIDGET_H
