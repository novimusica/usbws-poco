AM_LDFLAGS = -L@libdir@ \
	     -lPocoFoundation -lPocoUtil -lPocoNet -lPocoNetSSL -lstdc++

if !WITH_LIBUSB
AM_CPPFLAGS =
LDFLAGS_CMD = -lusbip -lusbipc
LDFLAGS_DAEMON_DEV = -lusbip -lusbipd
LDFLAGS_DAEMON_APP = -lusbip -lusbipa

sbin_PROGRAMS = usbws usbwsd usbwsa
else
AM_CPPFLAGS = -DUSBIP_WITH_LIBUSB
LDFLAGS_CMD = -lusbip_libusb -lusbip_stub -lusbipc_libusb
LDFLAGS_DAEMON_DEV = -lusbip_libusb -lusbip_stub -lusbipd_libusb
LDFLAGS_DAEMON_APP =

sbin_PROGRAMS = usbws usbwsd
endif

usbws_SOURCES = USBWSCommand.cpp USBWSWebSocket.cpp
usbws_CPPFLAGS = $(AM_CPPFLAGS)
usbws_LDFLAGS = $(AM_LDFLAGS) $(LDFLAGS_CMD)

usbwsd_SOURCES = USBWSDaemon.cpp USBWSWebSocket.cpp \
		USBWSRequestHandlerFactory.cpp USBWSRequestHandler.cpp
usbwsd_CPPFLAGS = $(AM_CPPFLAGS)
usbwsd_LDFLAGS = $(AM_LDFLAGS) $(LDFLAGS_DAEMON_DEV)

if !WITH_LIBUSB
usbwsa_SOURCES = USBWSDaemon.cpp USBWSWebSocket.cpp \
		USBWSRequestHandlerFactory.cpp USBWSRequestHandler.cpp
usbwsa_CPPFLAGS = $(AM_CPPFLAGS)
usbwsa_LDFLAGS = $(AM_LDFLAGS) $(LDFLAGS_DAEMON_APP)
endif
