QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BinaryFunction/binaryfunction.cpp \
    BinaryOperation/BinaryOperationAnd/binaryoperationand.cpp \
    BinaryOperation/BinaryOperationNot/binaryoperationnot.cpp \
    BinaryOperation/BinaryOperationOr/binaryoperationor.cpp \
    SBDD/binarynode.cpp \
    SBDD/sbdd.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    BinaryFunction/binaryfunction.h \
    BinaryOperation/BinaryOperationAnd/binaryoperationand.h \
    BinaryOperation/BinaryOperationNot/binaryoperationnot.h \
    BinaryOperation/BinaryOperationOr/binaryoperationor.h \
    BinaryOperation/abstractbinaryoperation.h \
    SBDD/binarynode.h \
    SBDD/sbdd.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
