#QT -= gui
QT += gui network qml


# For Android
android {
   QT += webview
}

# For iOS
ios {
    QT += webview
}

# For other platforms
!android:!ios {
    QT += webenginewidgets
}



#CONFIG += qmltypes
#QML_IMPORT_NAME = com.sarakata.skqtauth
#QML_IMPORT_MAJOR_VERSION = 1
TEMPLATE = lib
DEFINES += SKQTFIREBASE_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    skqtauth.cpp \
    skqtrealtimedatabase.cpp \
    skqtstorage.cpp

HEADERS += \
    SKQtFirebase_global.h \
    skqtauth.h \
    skqtrealtimedatabase.h \
    skqtstorage.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target



