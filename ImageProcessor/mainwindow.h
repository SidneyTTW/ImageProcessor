#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QAction>
#include <QMap>
#include <QSignalMapper>
#include <QVector>

#include "abstractcolorchooser.h"
#include "area.h"

class AbstractImageProcessor;
class AbstractImageProcessorWithSimpleOption;
class ImageProcessorWithComplexOptionAction;
class ImageProcessorWithSimpleOptionAction;
class ImageViewWidget;
class ProcessChain;
class SimpleOptionContainerWidget;

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow, public AbstractColorChooser
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  /**
   * Set current selected area.
   *
   * @param area The area.
   */
  inline void setArea(Area area)
  {
    _area = area;
  }

  /**
   * @return Current selected area.
   */
  inline Area getArea()
  {
    return _area;
  }

  virtual QColor getCurrentColor() const;
  virtual QColor getBackgroundColor() const;

protected:

private:
  Ui::MainWindow *ui;

  /**
   * Used to map signals of the simple actions.
   */
  QSignalMapper *signalMapper1;

  /**
   * Used to map signals of the complex actions.
   */
  QSignalMapper *signalMapper2;

  /**
   * Used to map signals of the change window.
   */
  QSignalMapper *signalMapper3;

  /**
   * Vector to store actions.
   */
  QVector<ImageProcessorWithSimpleOptionAction *> simpleActions;

  /**
   * Vector to store actions.
   */
  QVector<ImageProcessorWithComplexOptionAction *> complexActions;

  /**
   * A widget which contains option widget.
   */
  SimpleOptionContainerWidget *simpleOptionContainerWidget;

  /**
   * Current selected area.
   */
  Area _area;

  /**
   * Disconnect all the processor and the image.
   */
  void disconnectAll();

  /**
   * Disconnect all the processor except given one and the image.
   */
  void disconnectAllExcept(ImageProcessorWithSimpleOptionAction *action);

  /**
   * Connect a simple action with the image.
   */
  void connectSimpleAction(ImageProcessorWithSimpleOptionAction *action);

  /**
   * Map from the widget to the process chain.
   */
  QMap <ImageViewWidget *, ProcessChain *> processChains;

  /**
   * Map from the widget to the action.
   */
  QMap <ImageViewWidget *, QAction *> actions;

  /**
   * @return Current image view widget.
   */
  ImageViewWidget *currentWidget();

  /**
   * @return Current process chain.
   */
  ProcessChain *currentChain();

private slots:
  /**
   * Slot used to accept the selection of simple actions.
   */
  void on_compressButton_clicked();
  void on_closeButton_clicked();
  void simpleActionSelected(QObject *action);

  /**
   * Slot used to accept the selection of complex actions.
   */
  void complexActionSelected(QObject *action);

public slots:
  /**
   * Called to preview the result when the option of a simple processor is
   * changed.
   */
  void simpleOptionChanged(AbstractImageProcessorWithSimpleOption *processor);

  /**
   * Undo.
   */
  void undo();

  /**
   * Redo.
   */
  void redo();

  /**
   * Add a simple processor.
   */
  void addSimpleProcessor(AbstractImageProcessor *processor);

  /**
   * Add a complex processor.
   */
  void addComplexProcessor(AbstractImageProcessor *processor);

  /**
   * Add a processor.
   */
  void addProcessor(AbstractImageProcessor *processor);

  /**
   * Open.
   */
  void open();

  /**
   * Save.
   */
  void saveAs();

  /**
   * Save process chain.
   */
  void saveChain();

  /**
   * Change to the widget.
   */
  void changeToWidget(QObject *widget);
};

#endif // MAINWINDOW_H
