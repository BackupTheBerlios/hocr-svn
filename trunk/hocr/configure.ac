dnl Process this file with autoconf to produce a configure script.

AC_INIT(configure.in)
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE(hocr, 0.8.2)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

VERSION_INFO=0:0:0
AC_SUBST(VERSION_INFO)

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AM_PROG_LIBTOOL

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

PACKAGE_CFLAGS=-Wall
PACKAGE_LIBS=-lm

win32=no
case "${host_os}" in
  *mingw* | pw32* | cygwin*)
	win32="yes"
    AC_CHECK_TOOL(WINDRES, windres, :)
    PACKAGE_CFLAGS="$PACKAGE_CFLAGS -mms-bitfields -mno-cygwin"
esac
AM_CONDITIONAL(WITH_WIN32, test x"$win32" = "xyes")

AC_ARG_ENABLE(nls, [  --disable-nls		disable native language support], nls="$enableval", nls=yes)

if test "x$nls" = "xyes"; then
	GETTEXT_PACKAGE=hocr-gtk
	AC_SUBST(GETTEXT_PACKAGE)
	AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package.])
	
	dnl Add the languages which your application supports here.
	ALL_LINGUAS="he sv"
	AM_GLIB_GNU_GETTEXT
	
	PO_DIR=po
	AC_SUBST(PO_DIR)
fi

dnl =======================================================================================

build_gtk=yes
gtk=no

AC_ARG_ENABLE(gtk, [  --disable-gtk		do not build hocr-gtk program], build_gtk="$enableval", build_gtk=yes)

if test "x$build_gtk" = "xyes"; then
	gtk_pkg_modules="gtk+-2.0 gthread-2.0"
	PKG_CHECK_MODULES(gtk, [$gtk_pkg_modules], gtk=yes, gtk=no)
	AC_SUBST(gtk_CFLAGS)
	AC_SUBST(gtk_LIBS)
fi

AM_CONDITIONAL(WITH_GTK, test "x$gtk" = "xyes")

dnl =======================================================================================

build_gtkspell=yes
gtkspell=no

AC_ARG_ENABLE(gtkspell, [  --disable-gtkspell	do not build hocr-gtk spellchecker], build_gtkspell="$enableval", build_gtkspell=yes)

if test "x$gtk" = "xyes"; then
	if test "x$build_gtkspell" = "xyes"; then
		gtkspell_pkg_modules="gtkspell-2.0"
		PKG_CHECK_MODULES(gtkspell, [$gtkspell_pkg_modules], gtkspell=yes, gtkspell=no)
		AC_SUBST(gtkspell_CFLAGS)
		AC_SUBST(gtkspell_LIBS)
	fi
fi

AM_CONDITIONAL(WITH_GTKSPELL, test "x$gtkspell" = "xyes")
AC_SUBST(WITH_GTKSPELL)

dnl =======================================================================================

build_cmd=yes
cmd=no

AC_ARG_ENABLE(cmd, [  --disable-cmd		do not build hocr program], build_cmd="$enableval", build_cmd=yes)

if test "x$build_cmd" = "xyes"; then
	cmd=yes
fi

AM_CONDITIONAL(WITH_CMD, test "x$cmd" = "xyes")

dnl =======================================================================================

build_glib=yes
glib=no

AC_ARG_ENABLE(glib, [  --disable-glib        do not build glib support], build_glib="$enableval", build_glib=yes)

if test "x$build_glib" = "xyes"; then
	glib_pkg_modules="glib-2.0"
	PKG_CHECK_MODULES(glib, [$glib_pkg_modules], glib=yes, glib=no)
	AC_SUBST(glib_CFLAGS)
	AC_SUBST(glib_LIBS)
fi

AM_CONDITIONAL(WITH_GLIB, test "x$glib" = "xyes")

dnl =======================================================================================

build_hspell=no

AC_ARG_ENABLE(hspell, [  --disable-hspell      enable the hspell backend [default=auto]], check_hspell="$enableval", check_hspell=yes)

AC_ARG_WITH(hspell-prefix, [  --with-hspell-prefix=DIR
                          specify under which prefix hspell is installed.], with_hspell_prefix="$withval", )

dnl change to pkg-config when hspell provides a hspell.pc file

dnl if no glib then can't use iconv to convert utf-8 to iso-8859-8 for hspell
if test "x$glib" = "xno"; then
	check_hspell=no
fi

if test "x$check_hspell" != "xno"; then
   saved_LDFLAGS=$PACKAGE_LDFLAGS

   hspell_CFLAGS=
   hspell_LIBS=" -lhspell -lz"

   if test "x$with_hspell_prefix" != "x"; then       
       PACKAGE_LDFLAGS="-L$with_hspell_prefix/lib "$PACKAGE_LDFLAGS
       hspell_CFLAGS="-I$with_hspell_prefix/include"
       hspell_LIBS="-L$with_hspell_prefix/lib "$hspell_LIBS
   fi

   AC_CHECK_LIB(hspell, hspell_get_dictionary_path, build_hspell=yes, build_hspell=no, -lz)
   PACKAGE_LDFLAGS=$saved_LDFLAGS

   AC_SUBST(hspell_CFLAGS)
   AC_SUBST(hspell_LIBS)
fi

AM_CONDITIONAL(WITH_HSPELL, test "$build_hspell" = yes)

dnl =======================================================================================

dnl bindings 
dnl =======================================================================================

dnl =======================================================================================

AC_CHECK_PROG(have_swig, "swig", yes, no)

dnl =======================================================================================

dnl =======================================================================================

build_python=no

AC_ARG_ENABLE(python, [  --enable-python	build python binding [default=no]], build_python="$enableval", build_python=no)

AC_ARG_WITH(python-sitelib-dir, [  --with-python-sitelib-dir=PATH	path to python site lib ])

AC_CHECK_PROG(have_python, "python", yes, no)

if test "x$have_python" = "xyes"; then
	  PY_PREFIX=`python -c 'import sys ; print sys.prefix'`
	  PY_VERSION=`python -c 'import sys ; print sys.version[[0:3]]'`
	  AC_MSG_CHECKING(for $PY_PREFIX/include/python$PY_VERSION/Python.h)
	  if test -f $PY_PREFIX/include/python$PY_VERSION/Python.h; then
		  AC_MSG_RESULT(yes)
		  PY_CFLAGS="-I$PY_PREFIX/include/python$PY_VERSION"
		  
		  if test "x$with_python_sitelib_dir" = "x" ; then
		    PY_MODULES_PATH="$PY_PREFIX/lib/python$PY_VERSION/site-packages"
		  else
		    PY_MODULES_PATH=$with_python_sitelib_dir
		  fi
		  
		  AC_SUBST([PY_VERSION])
		  AC_SUBST([PY_CFLAGS])
		  AC_SUBST([PY_MODULES_PATH])
		  if test "x$have_swig" = "xno"; then
			build_python=no
		  fi
	  else
		  AC_MSG_RESULT(no)
		  build_python=no
	  fi
fi

AM_CONDITIONAL(WITH_PYTHON, test "x$build_python" = "xyes")

dnl =======================================================================================

build_perl=no

AC_ARG_ENABLE(perl, [  --enable-perl		build perl binding [default=no]], build_perl="$enableval", build_perl=no)

AC_ARG_WITH(perl-sitelib-dir, [  --with-perl-sitelib-dir=PATH		path to perl site lib ])

AC_CHECK_PROG(have_perl, "perl", yes, no)

if test "x$have_perl" = "xyes"; then
	  PE_PREFIX=`(perl -e 'use Config; print $Config{archlib};')`
	  AC_MSG_CHECKING(for $PE_PREFIX/CORE/perl.h)
	  if test -f $PE_PREFIX/CORE/perl.h; then
		  AC_MSG_RESULT(yes)
		  PE_CFLAGS="-I$PE_PREFIX/CORE "
		  PE_CFLAGS=$PE_CFLAGS`(perl -e 'use Config; print $Config{cppflags};')`
		  
		  if test "x$with_perl_sitelib_dir" = "x" ; then
		    PE_MODULES_PATH=`(perl -e 'use Config; print $Config{sitelib};')`
		  else
		    PE_MODULES_PATH=$with_perl_sitelib_dir
		  fi
		  		  
		  AC_SUBST([PE_CFLAGS])
		  AC_SUBST([PE_MODULES_PATH])
		  if test "x$have_swig" = "xno"; then
			build_perl=no
		  fi
	  else
		  AC_MSG_RESULT(no)
		  build_perl=no
	  fi
fi

dnl FIXME: on fedora PE_MODULES_PATH="/usr/lib/perl5/site_perl"

AM_CONDITIONAL(WITH_PERL, test "x$build_perl" = "xyes")

dnl =======================================================================================

build_java=no
have_java=no

AC_ARG_ENABLE(java, [  --enable-java	build java binding [default=no]], build_java="$enableval", build_java=no)

AC_ARG_WITH(java-sdk-dir, [  --with-java-sdk-dir=PATH	path to java sdk ])
AC_ARG_WITH(java-imagej-dir, [  --with-imagej-dir=PATH	path to imagej source ])

JAVA_DEV_KIT_PATH=$with_java_sdk_dir
AC_SUBST([JAVA_DEV_KIT_PATH])

IMAGEJ_SOURCE_PATH=$with_imagej_dir
AC_SUBST([IMAGEJ_SOURCE_PATH])

if test "x$with_java_sdk_dir" = "x"; then
         build_java=no 
fi

if test "x$with_imagej_dir" = "x"; then
         build_java=no 
fi

if test -f ${with_java_sdk_dir}/bin/javac; then
         have_java=yes
fi

if test "x$have_java" = "xno"; then
         build_java=no 
fi

AM_CONDITIONAL(WITH_JAVA, test "x$build_java" = "xyes")

dnl =======================================================================================

dnl =======================================================================================

AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)

dnl =======================================================================================

AC_OUTPUT([
Makefile
libhocr.pc
src/Makefile
po/Makefile.in
tests/Makefile
examples/Makefile
examples/hocr-gtk/Makefile
examples/hocr/Makefile
examples/bindings/Makefile
examples/bindings/python/Makefile
examples/bindings/python/simple/Makefile
examples/bindings/python/pygtk/Makefile
examples/bindings/perl/Makefile
examples/bindings/pascal/Makefile
examples/bindings/java/Makefile
examples/bindings/java/imagej-plugin/Makefile
bindings/Makefile
bindings/python/Makefile
bindings/perl/Makefile
])

dnl ===========================================================================================
echo "
$PACKAGE-$VERSION

	prefix:				${prefix}
	compiler:			${CC}
	native language support		${nls}
	
	have swig			${have_swig}
	have python			${have_python}
	have perl			${have_perl}
	hava java			${have_java}
		javac			${with_java_sdk_dir}/bin/javac

	build hocr			${cmd}
	build hocr-gtk			${gtk}
		with spellchecker	${gtkspell}
	build glib support		${glib}
	build hspell backend		${build_hspell}
	
	build python binding		${build_python}
	build perl binding		${build_perl}
	build java binding		${build_java}
	
	perl site lib path:		${PE_MODULES_PATH}
	python site lib path:		${PY_MODULES_PATH}
	java sdk path:			${JAVA_DEV_KIT_PATH}
	imagej source path:		${IMAGEJ_SOURCE_PATH}

"
