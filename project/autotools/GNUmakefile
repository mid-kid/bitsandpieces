AHFILES := autoconf.h.in

BUILD := build

.PHONY: $(BUILD)
$(BUILD): | $(BUILD)/Makefile
	+@$(MAKE) --no-print-directory -C $(BUILD) $(MAKECMDGOALS)

$(lastword $(MAKEFILE_LIST)): ;
%:: $(BUILD) ;

.PHONY: autoclean
autoclean:
	rm -rf $(BUILD)
	rm -rf build-aux m4
	rm -rf autom4te.cache aclocal.m4
	rm -f configure configure~ Makefile.in
	rm -f $(AHFILES) $(addsuffix ~,$(AHFILES))

$(BUILD)/Makefile: | configure Makefile.in
	mkdir -p $(BUILD)
	cd $(BUILD) && ../configure

configure: | configure.ac aclocal.m4
	autoconf

Makefile.in: | Makefile.am configure.ac aclocal.m4 Makefile.am $(AHFILES)
	automake -a

$(AHFILES): | configure.ac aclocal.m4
	autoheader

aclocal.m4: | configure.ac
	libtoolize -q
	aclocal

configure.ac: ;
Makefile.am: ;
