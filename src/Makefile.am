#
# This file is part of sharing-plugin-template
#
# Copyright (C) 2008-2009 Nokia Corporation. All rights reserved.
#
# This maemo code example is licensed under a MIT-style license,
# that can be found in the file called "COPYING" in the root
# directory.
#

plugins_LTLIBRARIES = libyandexfotki.la

libyandexfotki_la_SOURCES = plugin.c common.cc send.c validate.c base64.cpp CP_RSA.cpp

libyandexfotki_la_CXXFLAGS = $(PACKAGE_CFLAGS) $(DEBUG_CFLAGS)
libyandexfotki_la_CFLAGS = $(PACKAGE_CFLAGS) $(DEBUG_CFLAGS)
libyandexfotki_la_LDFLAGS = -module -avoid-version -export-dynamic
libyandexfotki_la_LIBADD = $(PACKAGE_LIBS)


MAINTAINERCLEANFILES = Makefile.in


