
HEADERS += \
    $$PWD/libev_eventdispatcher/src/eventdispatcher_libev.h \
    $$PWD/libev_eventdispatcher/src/eventdispatcher_libev_p.h

SOURCES += \
    $$PWD/libev_eventdispatcher/src/eventdispatcher_libev.cpp \
    $$PWD/libev_eventdispatcher/src/eventdispatcher_libev_p.cpp \
    $$PWD/libev_eventdispatcher/src/timers_p.cpp \
    $$PWD/libev_eventdispatcher/src/socknot_p.cpp



win32 {
	HEADERS += $$PWD/libev_eventdispatcher/src/win32_utils.h
	SOURCES += $$PWD/libev_eventdispatcher/src/win32_utils.cpp
}

unix {
	system('pkg-config --exists libev') {
		CONFIG    += link_pkgconfig
		PKGCONFIG += libev
	}
	else {
		LIBS += -lev
	}

}
else {
	LIBS        += -lev
}

