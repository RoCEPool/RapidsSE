# Custom makefile to only build TCC if the library has not yet been compiled.

# Explicitly define the shell to be used for this makefile. 
SHELL := /bin/bash

CURRENT_PATH = ${PWD}
BINDIR := $(CURRENT_PATH)/bin
LIBDIR := $(CURRENT_PATH)/lib
LIBTCC := $(LIBDIR)/libtcc.a
LIBTCC1 := $(LIBDIR)/tcc/libtcc1.a


all : $(LIBTCC) 

$(LIBTCC) :
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	cd src && ./configure --prefix=./.. --libdir=./../lib --bindir=./../bin
	cd src && make -e --no-print-directory && make install

clean :
	cd src && make clean
	rm    $(LIBDIR)/*.a
	rm -r $(LIBDIR)/tcc
	rm    $(BINDIR)/tcc
