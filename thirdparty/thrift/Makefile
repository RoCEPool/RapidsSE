# Custom makefile to only build Thrift if the library has not yet been compiled.

# Explicitly define the shell to be used for this makefile. 
SHELL := /bin/bash

CURRENT_PATH = ${PWD}

BINDIR := $(CURRENT_PATH)/bin
LIBDIR := $(CURRENT_PATH)/lib
INCDIR := $(CURRENT_PATH)/include
BOOSTDIR := $(CURRENT_PATH)/boost_1_80_0

LIBTHRIFT := $(LIBDIR)/libthrift.a

THRIFT_VER := thrift-0.17.0
SRCTARBALL := $(CURRENT_PATH)/$(THRIFT_VER).tar.gz
SRCDIR     := $(THRIFT_VER)

all : $(LIBTHRIFT)

$(LIBTHRIFT) :
	@echo ""
	@echo "Compiling 3rd-party library: Thrift"
	@echo ""
	@echo "Extracting Boost dependency"
	@tar xvzf boost_dependency_1_80_0.tar.gz &>/dev/null
	@echo "Extracting Thrift"
	@tar xvzf thrift-0.17.0.tar.gz &>/dev/null
	@./configureThrift.sh $(SRCDIR)
	@echo "Compiling Thrift"
	+cd $(SRCDIR) && $(MAKE) && $(MAKE) install
	@rm -rf $(SRCDIR)
	@rm -rf $(BOOSTDIR)

# Empty rules
$(INCDIR) :

clean :
	@rm -rf $(BOOSTDIR)
	@rm -rf $(SRCDIR)
	@rm -rf $(LIBDIR)
	@rm -rf $(INCDIR)
	@rm -rf $(BINDIR)
	@rm -f configure.log
