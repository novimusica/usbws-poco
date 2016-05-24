AM_CPPFLAGS = -I$(top_srcdir)/../libsrc -I$(top_srcdir)/../src

bin_PROGRAMS = usbws usbwsa usbwsd

usbws_SOURCES = USBWSCommand.cpp USBWSWebSocket.cpp
usbws_CPPFLAGS = $(AM_CPPFLAGS)
usbws_LDADD = $(top_builddir)/../libsrc/libusbiplib.la \
		$(top_builddir)/../src/libusbip.la
usbws_LDFLAGS = -lPocoFoundation -lPocoUtil -lPocoNet -lPocoNetSSL -lstdc++

usbwsa_SOURCES = USBWSDaemon.cpp USBWSWebSocket.cpp \
		USBWSRequestHandlerFactory.cpp USBWSRequestHandler.cpp
usbwsa_CPPFLAGS = $(AM_CPPFLAGS)
usbwsa_LDADD = $(top_builddir)/../libsrc/libusbiplib.la \
		$(top_builddir)/../src/libusbipa.la
usbwsa_LDFLAGS = -lPocoFoundation -lPocoUtil -lPocoNet -lPocoNetSSL -lstdc++

usbwsd_SOURCES = USBWSDaemon.cpp USBWSWebSocket.cpp \
		USBWSRequestHandlerFactory.cpp USBWSRequestHandler.cpp
usbwsd_CPPFLAGS = $(AM_CPPFLAGS)
usbwsd_LDADD = $(top_builddir)/../libsrc/libusbiplib.la \
		$(top_builddir)/../src/libusbipd.la
usbwsd_LDFLAGS = -lPocoFoundation -lPocoUtil -lPocoNet -lPocoNetSSL -lstdc++
