TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0 glib-2.0 gobject-2.0 gstreamer-app-1.0 gstreamer-pbutils-1.0
QMAKE_CXXFLAGS += -I input/json/
#LIBS +=
#PRE_TARGETDEPS += '-I input/json'
SOURCES += \
        gstreamerplayer.cpp \
        jsoncpp.cpp \
        main.cpp \
        statemachine.cpp

HEADERS += \
    gstreamerplayer.h \
    json-forwards.h \
    json.h \
    statemachine.h\


DISTFILES += \
    AnimationClips.json \
    TestPlayer/Video_SM.csv \
    TestPlayer/convert_csv_to_json.py

