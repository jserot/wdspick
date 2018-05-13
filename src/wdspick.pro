QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#CONFIG += console  # for debug under windows

HEADERS = hms.h astrometry.h orbentry.h wdsentry.h wdsfilter.h tcpclient.h config.h extractor.h browser.h wdspicker.h context.h mainwindow.h 
SOURCES = hms.cpp  astrometry.cpp orbentry.cpp wdsentry.cpp wdsfilter.cpp tcpclient.cpp config.cpp extractor.cpp browser.cpp wdspicker.cpp mainwindow.cpp context.cpp main.cpp
APP_QML_FILES.files = ../lib/data/wds.txt ../lib/data/orb.txt ../lib/data/adc.txt
APP_QML_FILES.files += ../dist/macos/wdspick.ini
APP_QML_FILES.path = Contents/Resources
QMAKE_BUNDLE_DATA += APP_QML_FILES
ICON = ../lib/img/icon.icns
RC_ICON = ../lib/img/icon.ico
