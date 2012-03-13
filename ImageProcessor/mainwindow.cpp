#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QDialog>
#include <QFileDialog>
#include "colorchooser.h"
#include "abstractimageprocessorwithsimpleoption.h"
#include "abstractimageprocessorwithdialogoption.h"
#include "imageprocessorwithsimpleoptionaction.h"
#include "imageprocessorwithcomplexoptionaction.h"
#include "imageviewwidget.h"
#include "processchain.h"
#include "simpleoptioncontainerwidget.h"
#include "processoraid.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _area(Area())
{
  ui->setupUi(this);

  while (ui->stackedWidget->currentWidget())
    ui->stackedWidget->removeWidget(ui->stackedWidget->currentWidget());
  ui->openAction->setShortcut(QKeySequence::Open);
  QList<QKeySequence> keyList;
  keyList.push_back(QKeySequence::Save);
  keyList.push_back(QKeySequence::SaveAs);
  ui->saveAsAction->setShortcuts(keyList);
  ui->undoAction->setShortcut(QKeySequence::Undo);
  ui->redoAction->setShortcut(QKeySequence::Redo);

  signalMapper1 = new QSignalMapper();
  signalMapper2 = new QSignalMapper();
  signalMapper3 = new QSignalMapper();

  QVector<AbstractImageProcessorWithSimpleOption *> simpleOptions =
      ProcessorAid::simpleOptions();
  for (int i = 0;i < simpleOptions.size();++i)
  {
    AbstractImageProcessorWithSimpleOption *processor = simpleOptions[i];
    processor->setColorChooser(this);
    connect(processor,
            SIGNAL(optionChanged(AbstractImageProcessorWithSimpleOption*)),
            this,
            SLOT(simpleOptionChanged(AbstractImageProcessorWithSimpleOption*)));
    connect(processor,
            SIGNAL(processorCreated(AbstractImageProcessor*)),
            this,
            SLOT(addSimpleProcessor(AbstractImageProcessor*)));
    ImageProcessorWithSimpleOptionAction *action =
        new ImageProcessorWithSimpleOptionAction(processor, this);
    action->setCheckable(true);
    //action->setShortcut(QKeySequence::Open);
    action->setStatusTip(processor->name());
    action->setText(processor->name());
    simpleActions.append(action);
    connect(action, SIGNAL(triggered()), signalMapper1, SLOT(map()));
    signalMapper1->setMapping(action, (QObject *)action);
    ui->mainToolBar->addAction(action);
  }
  QVector<AbstractImageProcessorWithDialogOption *> complexOptions =
      ProcessorAid::complexOptions();
  for (int i = 0;i < complexOptions.size();++i)
  {
    AbstractImageProcessorWithDialogOption *processor = complexOptions[i];
    connect(processor,
            SIGNAL(processorCreated(AbstractImageProcessor*)),
            this,
            SLOT(addComplexProcessor(AbstractImageProcessor*)));
    ImageProcessorWithComplexOptionAction *action =
        new ImageProcessorWithComplexOptionAction(processor, this);
    //action->setShortcut(QKeySequence::Open);
    action->setStatusTip(processor->name());
    action->setText(processor->name());
    complexActions.append(action);
    connect(action, SIGNAL(triggered()), signalMapper2, SLOT(map()));
    signalMapper2->setMapping(action, (QObject *)action);
    ui->menuComplex_Convertion->addAction(action);
  }

  connect(signalMapper1, SIGNAL(mapped(QObject*)), this,
          SLOT(simpleActionSelected(QObject*)));
  connect(signalMapper2, SIGNAL(mapped(QObject*)), this,
          SLOT(complexActionSelected(QObject*)));
  connect(signalMapper3, SIGNAL(mapped(QObject*)), this,
          SLOT(changeToWidget(QObject*)));
  simpleOptionContainerWidget = new SimpleOptionContainerWidget();
  simpleOptionContainerWidget->show();
  ui->mainToolBar->addWidget(simpleOptionContainerWidget);

  connect(ui->undoAction, SIGNAL(triggered()), this, SLOT(undo()));
  connect(ui->redoAction, SIGNAL(triggered()), this, SLOT(redo()));
  connect(ui->openAction, SIGNAL(triggered()), this, SLOT(open()));
  connect(ui->saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
  connect(ui->openChainAction, SIGNAL(triggered()), this, SLOT(openChain()));
  connect(ui->saveChainAction, SIGNAL(triggered()), this, SLOT(saveChain()));
}

QColor MainWindow::getCurrentColor() const
{
  return ui->colorWidget->getCurrentColor();
}

QColor MainWindow::getBackgroundColor() const
{
  return ui->colorWidget->getBackgroundColor();
}

void MainWindow::simpleActionSelected(QObject *action)
{
  disconnectAllExcept((ImageProcessorWithSimpleOptionAction *)action);
  connectSimpleAction((ImageProcessorWithSimpleOptionAction *)action);
}

void MainWindow::complexActionSelected(QObject *action)
{
  ImageViewWidget *widget = currentWidget();
  ProcessChain *processChain = currentChain();
  if (widget == NULL || processChain == NULL)
    return;

  QDialog *dialog = ((ImageProcessorWithComplexOptionAction *)action)->
                    getConfiguarionInstance()->
                    getOptionDialog(Area(), *processChain->getCurrentImage());
  if (dialog != NULL)
    dialog->exec();
}

void MainWindow::disconnectAll()
{
  ImageViewWidget *widget = currentWidget();
  ProcessChain *processChain = currentChain();
  for (int i = 0;i < simpleActions.size();++i)
  {
    simpleActions[i]->getConfiguarionInstance()->interrupt();
    simpleActions[i]->setChecked(false);
    if (widget != NULL)
      widget->removeEventFilter(simpleActions[i]->getConfiguarionInstance());
  }
  simpleOptionContainerWidget->clear();
  if (widget == NULL || processChain == NULL)
    return;
  widget->setImage(processChain->getCurrentImage()->getImage());
  update();
}

void MainWindow::disconnectAllExcept(ImageProcessorWithSimpleOptionAction *
                                     action)
{
  ImageViewWidget *widget = currentWidget();
  for (int i = 0;i < simpleActions.size();++i)
  {
    if (simpleActions[i] == action)
      continue;
    simpleActions[i]->getConfiguarionInstance()->interrupt();
    simpleActions[i]->setChecked(false);
    if (widget != NULL)
      widget->removeEventFilter(simpleActions[i]->getConfiguarionInstance());
  }
}

void MainWindow::connectSimpleAction(ImageProcessorWithSimpleOptionAction *
                                     action)
{
  ImageViewWidget *widget = currentWidget();
  ProcessChain *processChain = currentChain();
  if (action->isChecked())
  {
    simpleOptionContainerWidget->setWidget
        (action->getConfiguarionInstance()->optionWidget());
    if (widget == NULL || processChain == NULL)
      return;
    widget->setImage(action->getConfiguarionInstance()->preProcessImage
                     (*processChain->getCurrentImage()).getImage());
    update();
    widget->installEventFilter(action->getConfiguarionInstance());
  }
  else
  {
    simpleOptionContainerWidget->clear();
    if (widget == NULL || processChain == NULL)
      return;
    widget->setImage(processChain->getCurrentImage()->getImage());
    update();
  }
}

ImageViewWidget *MainWindow::currentWidget()
{
  return (ImageViewWidget *) ui->stackedWidget->currentWidget();
}

ProcessChain *MainWindow::currentChain()
{
  return processChains.value(currentWidget());
}

void MainWindow::simpleOptionChanged(AbstractImageProcessorWithSimpleOption *
                                     processor)
{
  ImageViewWidget *widget = currentWidget();
  ProcessChain *processChain = currentChain();
  if (widget == NULL || processChain == NULL)
    return;
  widget->setImage(processor->preProcessImage(
      (*processChain->getCurrentImage())).getImage());
  update();
}

void MainWindow::undo()
{
  ImageViewWidget *widget = currentWidget();
  ProcessChain *processChain = currentChain();
  if (widget == NULL || processChain == NULL)
    return;
  processChain->previous();
  disconnectAll();
  widget->setImage(processChain->getCurrentImage()->getImage());
  update();
}

void MainWindow::redo()
{
  ImageViewWidget *widget = currentWidget();
  ProcessChain *processChain = currentChain();
  if (widget == NULL || processChain == NULL)
    return;
  processChain->next();
  disconnectAll();
  widget->setImage(processChain->getCurrentImage()->getImage());
  update();
}

void MainWindow::addSimpleProcessor(AbstractImageProcessor * processor)
{
  addProcessor(processor);
  if (((AbstractImageProcessorWithSimpleOption *)processor)->
      cancelWhenNewOneIsCreated())
    disconnectAll();
}

void MainWindow::addComplexProcessor(AbstractImageProcessor * processor)
{
  addProcessor(processor);
  disconnectAll();
}

void MainWindow::addProcessor(AbstractImageProcessor *processor)
{
  ImageViewWidget *widget = currentWidget();
  ProcessChain *processChain = currentChain();
  if (widget == NULL || processChain == NULL)
  {
    delete processor;
    return;
  }
  processChain->addProcessorAtCurrentPosition(processor);
  widget->setImage(processChain->getCurrentImage()->getImage());
  update();
}

void MainWindow::open()
{
  QString path =
      QFileDialog::getOpenFileName(this,
                                   "Select an image file",
                                   tr(""),
                                   "Image (*.png *.jpg *.jpeg *.bmp)");
  if (path.isEmpty())
    return;
  disconnectAll();
  ImageViewWidget *widget = new ImageViewWidget();
  MyImage *image = new MyImage(QImage(QImage(path).
                                      convertToFormat(QImage::Format_ARGB32)),
                              MyImage::Colored);
  QAction *action = ui->windowMenu->addAction(path, signalMapper3, SLOT(map()));
  signalMapper3->setMapping(action, (QObject *)widget);
  widget->setImage(image->getImage());
  processChains.insert(widget, new ProcessChain(image));
  actions.insert(widget, action);
  ui->stackedWidget->addWidget(widget);
  ui->stackedWidget->setCurrentWidget(widget);
}

void MainWindow::saveAs()
{
  //TODO
}


void MainWindow::openChain()
{
  ImageViewWidget *widget = currentWidget();
  ProcessChain *processChain = currentChain();
  if (widget == NULL || processChain == NULL)
    return;
  QString path =
      QFileDialog::getOpenFileName(this,
                                   "Select a process chain file",
                                   tr(""),
                                   "Process Chain (*.pc)");
  if (path.isEmpty())
    return;
  disconnectAll();
  QList<AbstractImageProcessor *> chains = ProcessChain::loadProcessor(path);
  AbstractImageProcessor *processor;
  foreach(processor, chains)
    if (processor != NULL)
      processChain->addProcessorAtCurrentPosition(processor);
  widget->setImage(processChain->getCurrentImage()->getImage());
  update();
}

void MainWindow::saveChain()
{
  ProcessChain *processChain = currentChain();
  if (processChain == NULL)
    return;
  QString path = QFileDialog::getSaveFileName(this,
                                              "Export process chain",
                                              "",
                                              "Process Chain (*.pc)");
  if (path.isEmpty())
    return;
  processChain->save(path);
}

void MainWindow::changeToWidget(QObject *widget)
{
  if (widget == currentWidget() || widget == NULL)
    return;
  disconnectAll();
  ui->stackedWidget->setCurrentWidget((QWidget *)widget);
}

MainWindow::~MainWindow()
{
  delete ui;
  delete signalMapper1;
  delete signalMapper2;
  for (int i = 0;i < simpleActions.size();++i)
    delete simpleActions[i];
  for (int i = 0;i < complexActions.size();++i)
    delete complexActions[i];
}

void MainWindow::on_closeButton_clicked()
{
  ImageViewWidget *widget = currentWidget();
  ProcessChain *processChain = currentChain();
  if (widget == NULL || processChain == NULL)
    return;
  processChains.remove(widget);
  QAction *action = actions.value(widget, NULL);
  actions.remove(widget);
  if (action)
    delete action;
  delete widget;
  delete processChain;
}

void MainWindow::on_compressButton_clicked()
{
  ProcessChain *processChain = currentChain();
  if (processChain == NULL)
    return;
  processChain->compress();
}
