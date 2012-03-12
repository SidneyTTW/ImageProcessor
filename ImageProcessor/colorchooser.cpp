#include "colorchooser.h"

#include <QMouseEvent>
#include "colorpushbutton.h"

static const QColor colors[] =
{
  QColor(  0,   0,   0),
  QColor( 70,  70,  70),
  QColor(120, 120, 120),
  QColor(153,   0,  48),
  QColor(237,  28,  36),
  QColor(255, 126,   0),
  QColor(255, 194,  14),
  QColor(255, 242,   0),
  QColor(168, 230,  29),
  QColor( 34, 177,  76),
  QColor(  0, 183, 239),
  QColor( 77, 109, 243),
  QColor( 47,  54, 153),
  QColor(111,  49, 152),

  QColor(255, 255, 255),
  QColor(220, 220, 220),
  QColor(180, 180, 180),
  QColor(156,  90,  60),
  QColor(255, 163, 177),
  QColor(229, 170, 122),
  QColor(245, 228, 156),
  QColor(255, 249, 189),
  QColor(211, 249, 188),
  QColor(157, 187,  97),
  QColor(153, 217, 243),
  QColor(112, 154, 209),
  QColor( 84, 109, 142),
  QColor(181, 165, 213)
};

ColorChooser::ColorChooser(QWidget *parent) :
    QWidget(parent)
{
  gridLayout = new QGridLayout();
  foreButton = new ColorPushButton(QColor(0, 0, 0));
  gridLayout->addWidget(foreButton, 0, 0, 1, 1);
  backButton = new ColorPushButton(QColor(0, 0, 0));
  gridLayout->addWidget(backButton, 1, 0, 1, 1);
  for (int i = 0;i < COLOR_COUNT;++i)
  {
    colorButtons[i] = new ColorPushButton(colors[i]);
    gridLayout->addWidget(colorButtons[i],
                          i / (COLOR_COUNT / 2),
                          1 + i % (COLOR_COUNT / 2),
                          1,
                          1);
    colorButtons[i]->installEventFilter(this);
  }
  setLayout(gridLayout);
}

bool ColorChooser::eventFilter(QObject *object, QEvent *event)
{
  switch (event->type())
  {
  case QEvent::MouseButtonPress:
    if (((QMouseEvent *)event)->button() == Qt::LeftButton)
      foreButton->setColor(((ColorPushButton *)object)->getColor());
    else if (((QMouseEvent *)event)->button() == Qt::RightButton)
      backButton->setColor(((ColorPushButton *)object)->getColor());
    return false;
    break;
  default:
    return false;
    break;
  }
}

QColor ColorChooser::getCurrentColor() const
{
  return foreButton->getColor();
}

QColor ColorChooser::getBackgroundColor() const
{
  return backButton->getColor();
}

ColorChooser::~ColorChooser()
{
  for (int i = 0;i < COLOR_COUNT;++i)
    delete colorButtons[i];
  delete foreButton;
  delete backButton;
}
