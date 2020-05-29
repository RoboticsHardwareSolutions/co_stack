#! gmake

#
# Copyright (C) 2006 Laurent Bessard
# 
# This file is part of canfestival, a library implementing the canopen
# stack
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# 

CC = gcc
OPT_CFLAGS = -O2
CFLAGS = $(OPT_CFLAGS)
PROG_CFLAGS =  -fPIC
PREFIX = /usr/local
TARGET = unix
OS = Linux
CAN_DRIVER = can_vscom
TIMERS_DRIVER = timers_unix
ENABLE_DLL_DRIVERS=1
CAN_DLL_CFLAGS= -L'Linux x86-64' -lvs_can_api_x86-64
VSCAN_API_FILENAME=VSCAN_API_latest.zip

# define target specific environment
ifeq ($(OS),CYGWIN)
DLL_TYPE=dll
CAN_VSCOM_DLL_FLAGS=-Wl,--export-all-symbols -Wl,--exclude-libs,ALL
endif

ifeq ($(OS),MINGW32)
DLL_TYPE=dll
CAN_VSCOM_DLL_FLAGS=-Wl,--export-all-symbols -Wl,--exclude-libs,ALL
endif

ifeq ($(OS),Linux)
DLL_TYPE=so
CAN_VSCOM_DLL_FLAGS=-Wl,-soname,libcanfestival_$(CAN_DRIVER).so
endif

INCLUDES = -I../../include -I../../include/$(TARGET) -I../../include/$(CAN_DRIVER)

OBJS = $(CAN_DRIVER).o

ifeq ($(ENABLE_DLL_DRIVERS),1)
CFLAGS += -fPIC
DRIVER = libcanfestival_$(CAN_DRIVER).$(DLL_TYPE)
else
DRIVER = $(OBJS)
endif

TARGET_SOFILES = $(DESTDIR)$(PREFIX)/lib/$(DRIVER)

all: driver

vs_can_api.h:
	wget http://www.vscom.de/download/multiio/linux/driver/${VSCAN_API_FILENAME}
	@if which unzip &> /dev/null; then \
	unzip -o ${VSCAN_API_FILENAME}; \
	else \
	echo "No unzip found. Please decompress ${VSCAN_API_FILENAME} manually"; \
	exit 1; \
	fi;

driver: vs_can_api.h $(DRIVER)

%.o: %.c
	$(CC) $(CFLAGS) $(PROG_CFLAGS) ${PROGDEFINES} $(INCLUDES) -o $@ -c $<

libcanfestival_$(CAN_DRIVER).$(DLL_TYPE): $(OBJS)
	$(CC) $(PROG_CFLAGS) -shared $(CAN_VSCOM_DLL_FLAGS) -o $@ $< $(CAN_DLL_CFLAGS)

install: libcanfestival_$(CAN_DRIVER).so
	mkdir -p $(DESTDIR)$(PREFIX)/lib/
	cp $< $(DESTDIR)$(PREFIX)/lib/

uninstall:
	rm -f $(TARGET_SOFILES)

clean:
	rm -f $(OBJS)
	rm -f *.zip *.a *.so *.dll *.h *.lib
	rm -f CHANGES.TXT

mrproper: clean
