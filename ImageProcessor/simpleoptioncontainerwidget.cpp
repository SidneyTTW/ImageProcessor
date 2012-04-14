#include "simpleoptioncontainerwidget.h"

SimpleOptionContainerWidget::SimpleOptionContainerWidget(QWidget *parent) :
    QWidget(parent)
{
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
  setMinimumWidth(120);
  layout = new QStackedLayout();
  setLayout(layout);
}

void SimpleOptionContainerWidget::setWidget(QWidget *widget)
{
  while (layout->currentWidget())
    layout->removeWidget(layout->currentWidget());
  layout->addWidget(widget);
}

void SimpleOptionContainerWidget::clear()
{
  while (layout->currentWidget())
    layout->removeWidget(layout->currentWidget());
}

SimpleOptionContainerWidget::~SimpleOptionContainerWidget()
{
  delete layout;
}
