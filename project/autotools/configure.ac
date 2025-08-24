AC_PREREQ([2.69])
LT_PREREQ([2.4.6])
AC_INIT([project], [1.0])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([1.16.2 -Wall foreign subdir-objects])
AM_SILENT_RULES([yes])

AC_PROG_CC
AM_PROG_AR
LT_INIT([win32-dll])
PKG_INSTALLDIR

EXTRA_CFLAGS=""
AC_SUBST([EXTRA_CFLAGS])

# Default cflags
AS_IF([test "$GCC" = yes], [dnl
    EXTRA_CFLAGS="$EXTRA_CFLAGS -Wall -Wextra"])

# Set compiler standard
AC_CACHE_CHECK([whether the compiler supports -std=gnu23],
    [ac_cv_prog_cc_gnu23], [dnl
    save_CFLAGS="$CFLAGS"; CFLAGS="$CFLAGS -std=gnu23"
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM],
       [ac_cv_prog_cc_gnu23=yes], [ac_cv_prog_cc_gnu23=no])
    CFLAGS="$save_CFLAGS"])
AS_IF([test "x$ac_cv_prog_cc_gnu23" = xyes], [dnl
    EXTRA_CFLAGS="$EXTRA_CFLAGS -std=gnu23"])

AC_CONFIG_HEADERS([autoconf.h])
AC_CONFIG_FILES([Makefile project.pc])

AC_OUTPUT
