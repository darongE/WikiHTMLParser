#-------------------------------------------------
#
# Project created by QtCreator 2018-08-01T15:32:58
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WikipediaParser
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    Updater/updater.cpp \
    Updater/ihttpclient.cpp \
    Updater/ipostpreparestrategy.cpp \
    Updater/ipostpreparestrategyfactory.cpp \
    Updater/networkcookiejar.cpp \
    Updater/debug_memory/bool_array.cpp \
    Updater/debug_memory/debug_new.cpp \
    Updater/debug_memory/mem_pool_base.cpp \
    Updater/debug_memory/static_mem_pool.cpp \
    Updater/ihttpclient.cpp \
    Updater/ipostpreparestrategy.cpp \
    Updater/ipostpreparestrategyfactory.cpp \
    Updater/networkcookiejar.cpp \
    Updater/updater.cpp \
    main.cpp \
    mainwindow.cpp \
    Updater/ipostpreparestrategy.cpp \
    Updater/postpreparestrategymultipart.cpp \
    Updater/httpclient.cpp

HEADERS += \
        mainwindow.h \
    Updater/updater.h \
    Updater/ihttpclient.h \
    Updater/ipostpreparestrategy.h \
    Updater/ipostpreparestrategyfactory.h \
    Updater/networkcookiejar.h \
    Updater/every_cpp.h \
    Updater/debug_memory/_nvwa.h \
    Updater/debug_memory/bool_array.h \
    Updater/debug_memory/c++11.h \
    Updater/debug_memory/class_level_lock.h \
    Updater/debug_memory/cont_ptr_utils.h \
    Updater/debug_memory/debug_new.h \
    Updater/debug_memory/fast_mutex.h \
    Updater/debug_memory/fc_queue.h \
    Updater/debug_memory/fixed_mem_pool.h \
    Updater/debug_memory/mem_pool_base.h \
    Updater/debug_memory/object_level_lock.h \
    Updater/debug_memory/pctimer.h \
    Updater/debug_memory/set_assign.h \
    Updater/debug_memory/static_assert.h \
    Updater/debug_memory/static_mem_pool.h \
    Updater/debug_memory/type_traits.h \
    Updater/every_cpp.h \
    Updater/ihttpclient.h \
    Updater/ipostpreparestrategy.h \
    Updater/ipostpreparestrategyfactory.h \
    Updater/networkcookiejar.h \
    Updater/updater.h \
    mainwindow.h \
    Updater/ipostpreparestrategy.h \
    Updater/postpreparestrategymultipart.h \
    Updater/httpclient.h

FORMS += \
        mainwindow.ui \
    Updater/updater.ui
