## Process this file with automake to produce Makefile.in

SUBDIRS = src $(PO_DIR) tests examples bindings

EXTRA_DIST = \
	autogen.sh libhocr.pc.in libhocr.pc BUGS HACKING Doxyfile Doxyfile_cpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libhocr.pc

hocrdocdir = ${prefix}/share/doc/hocr
hocrdoc_DATA = \
	README \
	AUTHORS \
	COPYING \
	NEWS \
	ChangeLog \
	INSTALL \
	HACKING \
	BUGS

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(datadir)/pixmaps/; \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(datadir)/pixmaps/; \
	    fi \
	  done \
	fi

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(DESTDIR)$(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(DESTDIR)$(distdir)/pixmaps; \
	    fi \
	  done \
	fi
