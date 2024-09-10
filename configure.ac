AC_INIT(afids-xvd, 1.05)
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config/m4])
AC_CONFIG_FILES([afids_xvd/script/xvd])
AC_CONFIG_FILES([afids_xvd/script/setup_afids_xvd.csh afids_xvd/script/setup_afids_xvd.sh])
AFIDS_COMMON
AC_PATH_XTRA
AC_FIND_MOTIF
# Pass include and link directory, for use with building external libraries
AC_SUBST([X11_INCLUDE], [$x_includes])
AC_SUBST([X11_LIB], [$x_libraries])
# If we have GCC, use isystem on X_CFLAGS, so we can avoid warning messages
# about things we can't change
if test "x$GCC" = "xyes"; then
  if test -n "$x_includes"; then
    X_CFLAGS="-isystem$x_includes"
  fi
fi

# This set up all the source directories. Source for this is found in
# config/m4/afids_source_directory.m4 so we can include this in multiple
# configuration files.
AFIDS_XVD_SOURCE_DIRECTORY

#=================================================================
# Need to find VICAR RTL library
AC_VICAR_RTL(1)
if test "$have_vicar_rtl" = "no"; then
   AC_MSG_ERROR([
The VICAR RTL library is required by AFIDS. Try specifying
location using --with-vicar-rtl if configure couldn't find the
library. You can specify --with-vicar-rtl=build if you want to build your
own local copy of VICAR RTL.

You can also specify THIRDPARTY=build if you want to build a local
copy of all the libraries.
])
fi

AC_VICAR
if test "$have_vicar" = "no"; then
   AC_MSG_ERROR([
The VICAR program is required by AFIDS. Try specifying
location using --with-vicar-rtl if configure couldn't find the
library. You can specify --with-vicar-rtl=build if you want to build your
own local copy of VICAR.

You can also specify THIRDPARTY=build if you want to build a local
copy of all the libraries.
])
fi

if test "$with_motif" = "no"; then
   AC_MSG_ERROR([
Building AFIDS XVD requires that Motif is on the system. You need to
install this library before trying to build this.
])
fi

# Always build AFIDS_XVD
AM_CONDITIONAL([BUILD_AFIDS_XVD], [true])
AM_CONDITIONAL([BUILD_AFIDS], [false])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

#======================================================================
# Give a nice log message describing what has been configured, and which
# optional libraries are going to be used.
#======================================================================

LOC_MSG()
LOC_MSG([AFIDS XVD is now configured])
LOC_MSG()
LOC_MSG([  Installation directory:        ${prefix}])
LOC_MSG([  Build debug version:           ${enable_debug}])
LOC_MSG([  Fortran compiler:              ${F77} ${AM_FFLAGS}])
LOC_MSG([  C compiler:                    ${CC} ${CFLAGS}])
LOC_MSG([  CXX compiler:                  ${CXX} ${CXXFLAGS}])
