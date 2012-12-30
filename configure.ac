dnl Initial checks
AC_INIT(gen/main.c)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(eag, 2.6)
AM_SANITY_CHECK

dnl Checks for programs.
AC_PROG_INSTALL
AC_PROG_CC

dnl Check how to create and link shared libraries
AC_CANONICAL_HOST
AM_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(stdlib.h stdarg.h string.h limits.h malloc.h sys/time.h)

dnl Check for the path to X11
AC_PATH_X
AM_CONDITIONAL(DO_EDT, test x$no_x != xyes)
if test x$no_x = xyes; then
  echo "compiling EAG without support for syntax directed editors (No X11)"
else
  X11LDIR=${x_libraries}
  X11IDIR=${x_includes}
  AC_SUBST(X11IDIR)
  AC_SUBST(X11LDIR)
fi

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME

dnl Checks for library functions.
AC_TYPE_SIGNAL
AC_FUNC_VPRINTF

dnl Checks for libraries.
AC_CHECK_LIB(m, pow)
AC_PROG_MAKE_SET

AC_OUTPUT([
Makefile
doc/Makefile
exp/Makefile
examples/Makefile
literature/Makefile
gen/Makefile
libXedt/Makefile
libalib/Makefile
libeag/Makefile
libebs/Makefile
libedt/Makefile
])
