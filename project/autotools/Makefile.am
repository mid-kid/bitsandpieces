ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -DNDEBUG
AM_CFLAGS = $(EXTRA_CFLAGS)
AM_LDFLAGS = -no-undefined

bin_PROGRAMS = project
lib_LTLIBRARIES = libproject.la

project_LDADD = libproject.la
project_SOURCES = \
	source/main.c

libproject_la_LDFLAGS = -version-info 0:0:0
libproject_la_SOURCES = \
	source/lib.c

include_HEADERS = \
	source/lib.h

pkgconfig_DATA = \
	project.pc
