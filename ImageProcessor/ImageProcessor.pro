#-------------------------------------------------
#
# Project created by QtCreator 2012-03-08T10:25:35
#
#-------------------------------------------------

QT       += core gui

TARGET = ImageProcessor2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    area.cpp \
    ellipse.cpp \
    myimage.cpp \
    simpleoptioncontainerwidget.cpp \
    imageprocessorwithsimpleoptionaction.cpp \
    imagealgorithm.cpp \
    basicstatistic.cpp \
    tograyprocessor.cpp \
    tograyoptionwidget.cpp \
    imageviewwidget.cpp \
    borderlayout.cpp \
    processchain.cpp \
    drawlineprocessor.cpp \
    histogram.cpp \
    linechart.cpp \
    basicstatisticplot.cpp \
    toblackandwhiteprocessor.cpp \
    thresholditem.cpp \
    toblackandwhitedialog.cpp \
    xplotpicker.cpp \
    imageprocessorwithcomplexoptionaction.cpp \
    colorpushbutton.cpp \
    colorchooser.cpp \
    penprocessor.cpp \
    processoraid.cpp \
    batchconvertiondialog.cpp \
    reverseprocessor.cpp \
    histogramequalizationprocessor.cpp \
    histogramequalizationoptionwidget.cpp \
    basicstatisticwidget.cpp \
    resizeprocessor.cpp \
    resizedialog.cpp \
    filterprocessor.cpp \
    filterdialog.cpp \
    builtinconvolutiondialog.cpp \
    newfiledialog.cpp \
    areachooser.cpp \
    fastgaussprocessor.cpp \
    hslprocessor.cpp \
    hsldialog.cpp \
    sharpenprocessor.cpp

HEADERS  += mainwindow.h \
    area.h \
    ellipse.h \
    myimage.h \
    abstractimageprocessor.h \
    abstractimageprocessorwithdialogoption.h \
    abstractimageprocessorwithsimpleoption.h \
    myglobal.h \
    simpleoptioncontainerwidget.h \
    imageprocessorwithsimpleoptionaction.h \
    imagealgorithm.h \
    basicstatistic.h \
    tograyprocessor.h \
    tograyoptionwidget.h \
    abstractcolorchooser.h \
    imageviewwidget.h \
    borderlayout.h \
    processchain.h \
    drawlineprocessor.h \
    histogram.h \
    linechart.h \
    basicstatisticplot.h \
    toblackandwhiteprocessor.h \
    thresholditem.h \
    toblackandwhitedialog.h \
    xplotpicker.h \
    imageprocessorwithcomplexoptionaction.h \
    colorpushbutton.h \
    colorchooser.h \
    penprocessor.h \
    processoraid.h \
    batchconvertiondialog.h \
    reverseprocessor.h \
    histogramequalizationprocessor.h \
    histogramequalizationoptionwidget.h \
    basicstatisticwidget.h \
    resizeprocessor.h \
    resizedialog.h \
    filterprocessor.h \
    filterdialog.h \
    builtinconvolutiondialog.h \
    newfiledialog.h \
    areachooser.h \
    fastgaussprocessor.h \
    hslprocessor.h \
    hsldialog.h \
    sharpenprocessor.h

FORMS    += mainwindow.ui \
    tograyoptionwidget.ui \
    toblackandwhitedialog.ui \
    batchconvertiondialog.ui \
    histogramequalizationoptionwidget.ui \
    basicstatisticwidget.ui \
    resizedialog.ui \
    filterdialog.ui \
    builtinconvolutiondialog.ui \
    newfiledialog.ui \
    hsldialog.ui

INCLUDEPATH += c:/qwt
CONFIG += qwt

RESOURCES += \
    images.qrc
