# -*- shell-script -*-
#
# 
# $HEADER$
#

# OMPI_MPIEXT_example_CONFIG([action-if-found], [action-if-not-found])
# -----------------------------------------------------------
AC_DEFUN([OMPI_MPIEXT_XCLFrame_CONFIG],[
    AC_CONFIG_FILES([ompi/mpiext/XCLFrame/Makefile])
    AC_CONFIG_FILES([ompi/mpiext/XCLFrame/c/Makefile])
    AC_CONFIG_FILES([ompi/mpiext/XCLFrame/multiDeviceMgmt/Makefile])
    AC_CONFIG_FILES([ompi/mpiext/XCLFrame/c/binding/dvMgmt/Makefile])
    AC_CONFIG_FILES([ompi/mpiext/XCLFrame/c/binding/tskMgmt/Makefile])

    # This example can always build, so we just execute $1.
    $1
])

# only need to set this if the component needs init/finalize hooks
AC_DEFUN([OMPI_MPIEXT_XCLFrame_NEED_INIT], [1])
