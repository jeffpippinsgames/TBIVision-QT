QT += \
        quick \
        serialport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        QmlTBIDisplay.cpp \
        gary.cpp \
        main.cpp \
        mary.cpp \
        max.cpp \
        pixelclusterclass.cpp \
        pixelcolumnclass.cpp \
        tbiline.cpp \
        tbipoint.cpp \
        toby.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    QmlTBIDisplay.h \
    TBITopSurfaceLineVotingStructure.h \
    gary.h \
    mary.h \
    max.h \
    pixelclusterclass.h \
    pixelcolumnclass.h \
    pixelcolumnprocessreturn_t.h \
    pixelfundamental.h \
    tbiline.h \
    tbipoint.h \
    toby.h

unix:!macx: LIBS += -L$$PWD/../../../../../opt/pylon/lib/ -lpylonbase-6.2.0
unix:!macx: LIBS += -L$$PWD/../../../../../opt/pylon/lib/ -lpylonutility-6.2.0
unix:!macx: LIBS += -L$$PWD/../../../../../opt/pylon/lib/ -lGCBase_gcc_v3_1_Basler_pylon
unix:!macx: LIBS += -L$$PWD/../../../../../opt/pylon/lib/ -lGenApi_gcc_v3_1_Basler_pylon

INCLUDEPATH += $$PWD/../../../../../opt/pylon/include
DEPENDPATH += $$PWD/../../../../../opt/pylon/include

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lopencv_core
unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lopencv_imgproc

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include
INCLUDEPATH += $$PWD/../../../../../usr/local/include/opencv4
DEPENDPATH += $$PWD/../../../../../usr/local/include/opencv4
