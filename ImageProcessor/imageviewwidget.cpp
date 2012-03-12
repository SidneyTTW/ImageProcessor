#include "imageviewwidget.h"


#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include "borderlayout.h"f

ImageViewWidget::ImageViewWidget(QWidget *parent) :
    QWidget(parent)
{
  horizonBar = new QScrollBar(Qt::Horizontal);
  verticalBar = new QScrollBar(Qt::Vertical);
  connect(horizonBar, SIGNAL(valueChanged(int)), this, SLOT(hBarChangedSlot(int)));
  connect(verticalBar, SIGNAL(valueChanged(int)), this, SLOT(vBarChangedSlot(int)));
  BorderLayout *layout = new BorderLayout();
  layout->addWidget(horizonBar, BorderLayout::South);
  layout->addWidget(verticalBar, BorderLayout::East);
  setLayout(layout);

  scaleX = 1.0;
  scaleY = 1.0;
  ow = 0;
  oh = 0;
  cw = 0;
  ch = 0;

  synchronous();
}

void ImageViewWidget::setImage(const QImage &image)
{
  pixmap = QPixmap::fromImage(image);

  ow = pixmap.width();
  oh = pixmap.height();

  synchronous();

  update();
}

QPoint ImageViewWidget::toImagePosition(QPoint point)
{
  try
  {
    int w = width();
    int h = height();
    int sltx = 0;
    int slty = 0;
    int tltx = 0;
    int tlty = 0;
    int iw = w;
    if (verticalBar->isVisible())
      iw = w - verticalBar->width();
    int ih = h;
    if (horizonBar->isVisible())
      ih = h - horizonBar->height();

    if (horizonBar->isVisible())
    {
      sltx = (cw - iw) * horizonBar->value() / horizonBar->maximum();
    }
    else
    {
      iw = cw;
      tltx = (w - iw) / 2;
    }

    if (verticalBar->isVisible())
    {
      slty = (ch - ih) * verticalBar->value() / verticalBar->maximum();
    }
    else
    {
      ih = ch;
      tlty = (h - ih) / 2;
    }
    return QPoint((sltx / scaleX)+(point.x()-tltx)*(iw / scaleX)/iw,
                  (slty / scaleY)+(point.y()-tlty)*(ih / scaleY)/ih);
  } catch(...) {}
  return QPoint(0, 0);
}

void ImageViewWidget::synchronous()
{
  if (ow == 0 || oh == 0)
  {
    resetScrollBars();
    return;
  }
  cw = (int) (ow * scaleX);
  ch = (int) (oh * scaleY);
  resetScrollBars();
}

void ImageViewWidget::zoomIn()
{
  scaleX = scaleX / 0.8;
  scaleY = scaleY / 0.8;
  synchronous();
  verticalBar->setValue((int) (verticalBar->value() / 0.8));
  horizonBar->setValue((int) (horizonBar->value() / 0.8));
}

void ImageViewWidget::zoomOut()
{
  scaleX = scaleX * 0.8;
  scaleY = scaleY * 0.8;
  synchronous();
  verticalBar->setValue((int) (verticalBar->value() * 0.8));
  horizonBar->setValue((int) (horizonBar->value() * 0.8));
}

void ImageViewWidget::resume()
{
  scaleX = 1.0;
  scaleY = 1.0;
  synchronous();
}

void ImageViewWidget::clear()
{
  ow = 0;
  oh = 0;
  cw = 0;
  ch = 0;
  pixmap = QPixmap();
  horizonBar->setVisible(false);
  verticalBar->setVisible(false);
}


void ImageViewWidget::resetScrollBars()
{
  if (ow == 0 || oh == 0)
  {
    horizonBar->setVisible(false);
    verticalBar->setVisible(false);
    return;
  }
  if (cw > width())
  {
    if (horizonBar->value() > cw)
      horizonBar->setValue(cw);
    horizonBar->setMaximum(cw);
    horizonBar->setVisible(true);
  }
  else
  {
    horizonBar->setVisible(false);
  }
  if (ch > height())
  {
    if (verticalBar->value() > ch)
      verticalBar->setValue(ch);
    verticalBar->setMaximum(ch);
    verticalBar->setVisible(true);
  }
  else
  {
    verticalBar->setVisible(false);
  }
  update();
}

void ImageViewWidget::wheelEvent (QWheelEvent *event)
{
  if (event->modifiers().testFlag(Qt::ControlModifier))
  {
    if (event->delta() < 0)
      zoomOut();
    else
      zoomIn();
  }
  else
  {
    bool horizon = event->modifiers().testFlag(Qt::ShiftModifier);
    QScrollBar *bar = horizon ? horizonBar : verticalBar;
    if (!bar->isVisible())
      return;
    int delta = 0.1 * bar->maximum();
    if (event->delta() < 0)
      bar->setValue(qBound(0, bar->value() + delta, bar->maximum()));
    else
      bar->setValue(qBound(0, bar->value() - delta, bar->maximum()));
  }
}

void ImageViewWidget::paintEvent(QPaintEvent *event)
{
  if (ow == 0 || oh == 0)
    return;
  // Get the painter
  QPainter *painter = new QPainter(this);
  int w = width();
  int h = height();
  painter->fillRect(0, 0, w, h, QBrush(QColor(100, 100, 100)));
  int sltx = 0;
  int slty = 0;
  int tltx = 0;
  int tlty = 0;
  int iw = w;
  if (verticalBar->isVisible())
    iw = w - verticalBar->width();
  int ih = h;
  if (horizonBar->isVisible())
    ih = h - horizonBar->height();

  if (horizonBar->isVisible())
  {
    sltx = (cw - iw) * horizonBar->value() / horizonBar->maximum();
  }
  else
  {
    iw = cw;
    tltx = (w - iw) / 2;
  }

  if (verticalBar->isVisible())
  {
    slty = (ch - ih) * verticalBar->value() / verticalBar->maximum();
  }
  else
  {
    ih = ch;
    tlty = (h - ih) / 2;
  }
  painter->drawPixmap(tltx, tlty, iw, ih, pixmap,
                      (int) (sltx / scaleX), (int) (slty / scaleY),
                      (int) (iw / scaleX), (int) (ih / scaleY));

  // End the paint and release the space
  painter->end();
  delete painter;
}

void ImageViewWidget::resizeEvent(QResizeEvent *event)
{
  resetScrollBars();
}

void ImageViewWidget::hBarChangedSlot(int value)
{
  update();
}

void ImageViewWidget::vBarChangedSlot(int value)
{
  update();
}
