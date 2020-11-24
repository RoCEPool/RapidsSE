# Custom makefile to only build TCC if the library has not yet been compiled.

# Explicitly define the shell to be used for this makefile. 
SHELL := /bin/bash

CURRENT_PATH = ${PWD}
BINDIR := $(CURRENT_PATH)/bin
LIBDIR := $(CURRENT_PATH)/lib
LIBTCC := $(LIBDIR)/libtcc.a
LIBTCC1 := $(LIBDIR)/tcc/libtcc1.a
CPPFLAGS := 
unexport

all : $(LIBTCC) 

$(LIBTCC) :
	@echo ""
	@echo "Compiling 3rd-party library: TCC"
	@echo ""
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	cd src && ./configure --prefix=./.. --libdir=./../lib --bindir=./../bin
	+cd src && $(MAKE) --no-print-directory && $(MAKE) install

clean :
	cd src && $(MAKE) clean
	rm -f  $(LIBDIR)/*.a
	rm -rf $(LIBDIR)/tcc
	rm -f  $(BINDIR)/tcc
