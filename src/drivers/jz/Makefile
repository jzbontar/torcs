##############################################################################
#
#    file                 : Makefile
#    created              : Thu Jan 21 11:41:38 CET 2016
#    copyright            : (C) 2002 Jure Zbontar
#
##############################################################################
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
##############################################################################

ROBOT       = jz
MODULE      = ${ROBOT}.so
MODULEDIR   = drivers/${ROBOT}
SOURCES     = ${ROBOT}.cpp

SHIPDIR     = drivers/${ROBOT}
SHIP        = ${ROBOT}.xml pw-imprezawrc.rgb logo.rgb
SHIPSUBDIRS = 

PKGSUBDIRS  = ${SHIPSUBDIRS}
src-robots-jz_PKGFILES = $(shell find * -maxdepth 0 -type f -print)
src-robots-jz_PKGDIR   = ${PACKAGE}-${VERSION}/$(subst ${TORCS_BASE},,$(shell pwd))

LIBS        = -L/home/jure/torch/install/lib -lluajit -lluaT -lTH
COMPILFLAGS = -I/home/jure/torch/install/include/ -I/home/jure/torch/install/include/TH/

include ${MAKE_DEFAULT}
