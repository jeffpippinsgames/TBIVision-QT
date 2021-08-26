QT += \
        quick \
        serialport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        tbiclass_datadistributionset.cpp \
        tbiclass_dataset.cpp \
        tbiclass_line.cpp \
        tbiclass_pointfloat.cpp \
        tbiclass_pointint.cpp \
        tbiclass_ransac.cpp \
        tbiclass_threepointtrackingcontainer.cpp \
        tbiclass_threepointtrackingmanager.cpp \
        tbicore_gary.cpp \
        tbicore_mary.cpp \
        tbicore_max.cpp \
        tbicore_toby.cpp \
        tbiparameterclass_gausiandecluster.cpp \
        tbiparameterclass_imageintensity.cpp \
        tbiparameterclass_ranscaparms.cpp \
        tbiqml_display.cpp \
        tbiweldtype_baseclass.cpp \
        tbiweldtype_buttjoint.cpp \
        tbiweldtype_enumerator.cpp \
        tbiweldtype_vgroove.cpp

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
    tbiclass_datadistributionset.h \
    tbiclass_dataset.h \
    tbiclass_line.h \
    tbiclass_pixelfundamental.h \
    tbiclass_pointfloat.h \
    tbiclass_pointint.h \
    tbiclass_ransac.h \
    tbiclass_threepointtrackingcontainer.h \
    tbiclass_threepointtrackingmanager.h \
    tbicore_constants.h \
    tbicore_gary.h \
    tbicore_mary.h \
    tbicore_max.h \
    tbicore_toby.h \
    tbiparameterclass_gausiandecluster.h \
    tbiparameterclass_imageintensity.h \
    tbiparameterclass_ransacvotingstruct.h \
    tbiparameterclass_ranscaparms.h \
    tbiqml_display.h \
    tbiweldtype_baseclass.h \
    tbiweldtype_buttjoint.h \
    tbiweldtype_enumerator.h \
    tbiweldtype_vgroove.h

unix:!macx: LIBS += -L$$PWD/../../../../../opt/pylon/lib/ -lpylonbase-6.2.0
unix:!macx: LIBS += -L$$PWD/../../../../../opt/pylon/lib/ -lpylonutility-6.2.0
unix:!macx: LIBS += -L$$PWD/../../../../../opt/pylon/lib/ -lGCBase_gcc_v3_1_Basler_pylon
unix:!macx: LIBS += -L$$PWD/../../../../../opt/pylon/lib/ -lGenApi_gcc_v3_1_Basler_pylon

INCLUDEPATH += $$PWD/../../../../../opt/pylon/include
DEPENDPATH += $$PWD/../../../../../opt/pylon/include

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/ -lopencv_core
unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/ -lopencv_imgproc
unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/ -lopencv_imgcodecs

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include
INCLUDEPATH += $$PWD/../../../../../usr/include/opencv4
DEPENDPATH += $$PWD/../../../../../usr/include/opencv4

