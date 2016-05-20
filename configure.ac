dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([usbws-utils], [0.1.0], [linux-usb@vger.kernel.org])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign])
LT_INIT

# Silent build for automake >= 1.11
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_SUBST([EXTRA_CFLAGS], ["-Wall -Werror -Wextra -std=gnu99"])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Set language to C++
AC_LANG_CPLUSPLUS

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([fcntl.h unistd.h])

AC_CHECK_HEADER(Poco/Logger.h,,
	AC_MSG_ERROR([Missing Poco/Logger.h]))
AC_CHECK_HEADER(Poco/Util/ServerApplication.h,,
	AC_MSG_ERROR([Missing Poco/Util/ServerApplication.h]))
AC_CHECK_HEADER(Poco/Net/WebSocket.h,,
	AC_MSG_ERROR([Missing Poco/Net/WebSocket.h]))
AC_CHECK_HEADER(Poco/Net/Context.h,,
	AC_MSG_ERROR([Missing Poco/Net/Context.h]))
AC_CHECK_LIB(PocoFoundation,main,LIBS="$LIBS -lPocoFoundation",
	AC_MSG_ERROR([libPocoFoundation not found!]))
AC_CHECK_LIB(PocoUtil,main,LIBS="$LIBS -lPocoUtil",
	AC_MSG_ERROR([libPocoUtil not found!]))
AC_CHECK_LIB(PocoNet,main,LIBS="$LIBS -lPocoNet",
	AC_MSG_ERROR([libPocNet not found!]))
AC_CHECK_LIB(PocoNetSSL,main,LIBS="$LIBS -lPocoNetSSL",
	AC_MSG_ERROR([libPocNetSSL not found!]))

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
