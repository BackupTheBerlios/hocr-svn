## Makefile.am -- Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src examples bindings docs

EXTRA_DIST = \
        Doxyfile HACKING

pkgconfigdir = $(libdir)/pkgconfig

if WITH_GTK
pkgconfig_DATA = libhocr.pc libhocr-gtk.pc
else
pkgconfig_DATA = libhocr.pc
endif

hocrdocdir = ${prefix}/share/doc/libhocr
hocrdoc_DATA = \
        README \
        AUTHORS \
        COPYING \
        NEWS \
        ChangeLog \
        INSTALL \
        HACKING
