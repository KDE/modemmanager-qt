prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
libdir=@CMAKE_INSTALL_PREFIX@/lib@LIB_SUFFIX@
includedir=${prefix}/include

Name: libQtModemManager
Description: Convenience Qt library for clients of ModemManager
Version: @VERSION@

Requires: QtCore QtDBus QtXml
Cflags: -I${includedir} @CMAKE_INCLUDE_PATH@
Libs: -L${libdir} -lQtModemManager @CMAKE_LIBRARY_PATH@
