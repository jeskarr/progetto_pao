QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    model/base_elem.cpp \
    model/game.cpp \
    model/game_set.cpp \
    model/image_elem.cpp \
    model/memoryManagement.cpp \
    model/quote_elem.cpp \
    view/visitorInfo.cpp \
    view/editSetElement.cpp \
    view/endGame.cpp \
    view/gameArea.cpp \
    view/gameSettings.cpp \
    view/gameWindow.cpp \
    view/mainwindow.cpp \
    view/newSetElement.cpp \
    view/setCreationWindow.cpp \
    view/setEditWindow.cpp \
    view/setSelectionWindow.cpp

HEADERS += \
    model/base_elem.h \
    model/game.h \
    model/game_set.h \
    model/image_elem.h \
    model/memoryManagement.h \
    model/quote_elem.h \
    model/visitorInterface.h \
    view/visitorInfo.h \
    view/editSetElement.h \
    view/endGame.h \
    view/gameArea.h \
    view/gameSettings.h \
    view/gameWindow.h \
    view/mainwindow.h \
    view/newSetElement.h \
    view/setCreationWindow.h \
    view/setEditWindow.h \
    view/setSelectionWindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    style.qss
