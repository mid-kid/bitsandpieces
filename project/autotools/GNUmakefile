AHFILES := autoconf.h.in

ACTARGETS := clean distclean

.PHONY: all
all: | build/Makefile
	@$(MAKE) --no-print-directory -C build $(MAKECMDGOALS)

.PHONY: $(ACTARGETS)
$(ACTARGETS): all ;@:

.PHONY: autoclean
autoclean:
	rm -rf build
	rm -rf build-aux m4
	rm -rf autom4te.cache aclocal.m4
	rm -f configure configure~ Makefile.in
	rm -f $(AHFILES) $(addsuffix ~,$(AHFILES))

build/Makefile: | configure Makefile.in
	mkdir -p build
	cd build && ../configure

configure: | configure.ac aclocal.m4
	autoconf

Makefile.in: | Makefile.am configure.ac aclocal.m4 Makefile.am $(AHFILES)
	automake -a

$(AHFILES): | configure.ac aclocal.m4
	autoheader

aclocal.m4: | configure.ac
	libtoolize
	aclocal
