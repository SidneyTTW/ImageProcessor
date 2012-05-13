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
    sharpenprocessor.cpp \
    redeyereductionprocessor.cpp \
    algebraoperationdialog.cpp \
    algebraoperationprocessor.cpp \
    histogrammatchdialog.cpp \
    histogrammatchprocessor.cpp \
    pointprocessor.cpp \
    pointdialog.cpp \
    rotateprocessor.cpp \
    rotatedialog.cpp \
    eraseprocessor.cpp \
    magiceraseprocessor.cpp \
    boundprocessor.cpp \
    bounddialog.cpp \
    dilationerosiondialog.cpp \
    dilationerosionprocessor.cpp \
    rgbmapdialog.cpp \
    rgbmapplot.cpp \
    xyplotpicker.cpp \
    rgbmapprocessor.cpp

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
    sharpenprocessor.h \
    redeyereductionprocessor.h \
    algebraoperationdialog.h \
    algebraoperationprocessor.h \
    histogrammatchdialog.h \
    histogrammatchprocessor.h \
    pointprocessor.h \
    pointdialog.h \
    rotateprocessor.h \
    rotatedialog.h \
    eraseprocessor.h \
    magiceraseprocessor.h \
    boundprocessor.h \
    bounddialog.h \
    dilationerosiondialog.h \
    dilationerosionprocessor.h \
    rgbmapdialog.h \
    rgbmapplot.h \
    xyplotpicker.h \
    rgbmapprocessor.h

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
    hsldialog.ui \
    algebraoperationdialog.ui \
    histogrammatchdialog.ui \
    pointdialog.ui \
    rotatedialog.ui \
    bounddialog.ui \
    dilationerosiondialog.ui \
    rgbmapdialog.ui

INCLUDEPATH += c:/qwt
CONFIG += qwt

RESOURCES += \
    images.qrc
