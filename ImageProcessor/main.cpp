#include <QtGui/QApplication>
#include "mainwindow.h"

#include "rgbmapdialog.h"

int main(int argc, char *argv[])
{
  QApplication::setStyle("cleanlooks");
  QApplication a(argc, argv);
  MainWindow w;
//  RGBMapDialog w(QImage("J:/tmp.png").convertToFormat(QImage::Format_ARGB32), Area());
  w.show();

  return a.exec();
}
