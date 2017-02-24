######################################################################
# Automatically generated by qmake (2.01a) Tue Nov 3 10:00:05 2015
######################################################################

<code>CONFIG += debug               # enables debugging (like the -g flag)
TEMPLATE = app
TARGET = search
DEPENDPATH += .
INCLUDEPATH += .

# Input
-= crawler.cpp
HEADERS += mainwindow.h \
           msort.h \
           myset.h \
           pageparser.h \
           searcheng.h \
           util.h \
           webpage.h
SOURCES += mainwindow.cpp \
           pageparser.cpp \
           search.cpp \
           searcheng.cpp \
           util.cpp \
           webpage.cpp


QMAKE_CXXFLAGS += -std=c++11  # enable C++11 libraries
SOURCES -= msort_test.cpp # Don't compile certain test files as part of the search application</code>
SOURCES -= crawler.cpp