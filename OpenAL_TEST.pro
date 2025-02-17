QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        soundmanager.cpp

INCLUDEPATH += \
        D:/Repositories/OpenALSDK/include/

LIBS += -L"D:/Repositories/OpenALSDK/libs/Win64" -lOpenAL32


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#"D:/Repositories/OpenAL_TEST/sounds/15716__blackstrobe__a.wav",
#"D:/Repositories/OpenAL_TEST/sounds/15726__blackstrobe__k.wav",
#"D:/Repositories/OpenAL_TEST/sounds/15733__blackstrobe__morse_4.wav",
#"D:/Repositories/OpenAL_TEST/sounds/15736__blackstrobe__morse_7.wav"

HEADERS += \
    datasource.h \
    soundmanager.h \
    wavdata.h
