VisualStudio solution structure example

[For usbws and usbwsd]

$(SolutionDir)
|
+-lib
|   Type:    lib
|   Sources: usbip_network.c usbip_common.c usbip_host_nppi.c names.c
|            stub_*.c # from libusb/stub
|            dummy_device_driver.c # from libusb/libsrc
|   Headers: usbip_network.h usbip_common.h usbip_config.h
|            usbip_host_common.h usbip_host_driver.h usbip_device_driver.h
|            names.h list.h
|            stub_common.h stub.h # from libusb/stub
|            usbip_os.h usbip_sock.h # from libusb/os
|   Includes: $(SolutionDir);$(SolutionDir)\win32
|   Definitions: USBIP_WITH_LIBUSB;HAVE_CONFIG_H;USBIP_AS_LIBRARY
|
+-libusb-1.0 (dll)
|   Added as an existing project from <libusb-src>\msvc\libusb_dll_<ver>.vcxproj
|   OutputDir: $(SolutionDir)$(Configuration)\
|
+-usbip
|   Type:    lib
|   Sources: usbip_connect.c usbip_disconnect.c usbip_list.c
|   Headers: usbip.h
|   Includes: $(SolutionDir);$(SolutionDir)\lib;$(SolutionDir)\win32
|   Definitions: USBIP_WITH_LIBUSB;HAVE_CONFIG_H;USBIP_AS_LIBRARY;
|            USBIDS_FILE="..\\usb.ids"
|
+-usbipd
|   Type:    lib
|   Sources: usbipd_dev.c
|   Headers: usbipd.h
|   Includes: $(SolutionDir);$(SolutionDir)\lib;$(SolutionDir)\win32
|   Definitions: USBIP_WITH_LIBUSB;HAVE_CONFIG_H;USBIP_AS_LIBRARY
|
+-win32
|   Headers: usbip_os.h usbip_sock.h # from libusb/os/win32
|
+linux
|   Headers: usbip_nppi.h # from include/uapi/linux
|
+-config.h
|   Copied from libusb/os/win32
|
+-usb.ids
|   Copied from /usr/share/hwdata/ or downloaded from net.
|   AccentAcute character must be modified to avoid invalid ascii exception. 
|   ex) sed -i -e 's/\xb4/\x00/' usb.ids
|
+-usbws
|   Type:    exe
|   Sources: USBWSWebSocket.cpp
|            USBWSCommand.cpp
|   Headers: USBWSWebSocket.h USBWSUtil.h
|            USBWSCommand.h
|   Includes: $(SolutionDir)\lib
|            $(SolutionDir)\include
|            <poco-src>\Crypto\include
|            <poco-src>\Foundation\include
|            <poco-src>\Net\include
|            <poco-src>\NetSSL_OpenSSL\include
|            <poco-src>\Util\include
|            <OpenSSL-Win32>\include
|   Definitions: USBIP_WITH_LIBUSB
|   Depending Files: ws2_32.lib
|   Reference: lib;libusb-1.0 (dll);usbip;win32
|            Crypt;Foundation;JASON;Net;NetSSL_OpenSSL;Util;XML
|
+-usbwsd
|   Type:    exe
|   Sources: USBWSWebSocket.cpp
|            USBWSDaemon.cpp USBWSRequestHandler.cpp
|            USBWSRequestHandlerFactory.cpp
|   Headers: USBWSWebSocket.h USBWSUtil.h
|            USBWSDaemon.h USBWSRequestHandler.h
|            USBWSRequestHandlerFactory.h
|   Includes: $(SolutionDir)\lib
|            <poco-src>\Crypto\include
|            <poco-src>\Foundation\include
|            <poco-src>\Net\include
|            <poco-src>\NetSSL_OpenSSL\include
|            <poco-src>\Util\include
|            <OpenSSL-Win32>\include
|   Definitions: USBIP_WITH_LIBUSB
|   Depending Files: ws2_32.lib
|   Reference: lib;libusb-1.0 (dll);usbipd;win32
|            Crypt;Foundation;JASON;Net;NetSSL_OpenSSL;Util;XML
|
+-Crypto
+-Foundation
+-JASON
+-Net
+-NetSSL_OpenSSL
+-Util
+-XML
|   Type:    dll
|   Added as existing projects from <poco-src>\<sub>\<sub>_vs<ver>.vcxproj
|
+-libusb-1.0
|   SYMLINK to <libusb-src>\libusb
|   ex) mklink /D libusb-1.0 <libusb-src>\libusb
+-Debug/Release
    copy <poco-src>\bin\* .
    copy <OpenSSL-Win32>\bin\ssleay32.dll .
    copy <OpenSSL-Win32>\bin\libeay32.dll .

NOTE:
    To support Windows 7 and Visual Studio 2012, programs must not be c99.

    Winsock headers must be included at first than any other headers.
    Please, check the position which includes usbip_sock.h if duplicated
    definition errors relating  winsock headers occur.
