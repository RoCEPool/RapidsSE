# Custom makefile to only build Thrift if the library has not yet been compiled.

# Explicitly define the shell to be used for this makefile. 
SHELL := /bin/bash

CURRENT_PATH = ${PWD}

BINDIR := $(CURRENT_PATH)/bin
LIBDIR := $(CURRENT_PATH)/lib
INCDIR := $(CURRENT_PATH)/include
BOOSTDIR := $(CURRENT_PATH)/boost_1_80_0

LIBTHRIFT := $(LIBDIR)/libthrift.a

THRIFT_VER := thrift-0.18.1
SRCTARBALL := $(CURRENT_PATH)/$(THRIFT_VER).tar.gz
SRCDIR     := $(THRIFT_VER)

all : $(LIBTHRIFT) $(BOOSTDIR)

$(LIBTHRIFT) : $(BOOSTDIR)
	@echo ""
	@echo "Compiling 3rd-party library: Thrift"
	@echo ""
	@echo "Extracting Thrift"
	@tar xvzf $(THRIFT_VER).tar.gz &>/dev/null
	@./configureThrift.sh $(SRCDIR)
	@echo "Compiling Thrift"
	+cd $(SRCDIR) && $(MAKE) && $(MAKE) install
	@rm -rf $(SRCDIR)

$(BOOSTDIR) :
	@echo "Extracting Boost dependency"
	@tar xvzf boost_dependency_1_80_0.tar.gz &>/dev/null
# Note that we cannot clean up the boost directory because it is required to compile
# the wireline protocol messages.

# Empty rules
$(INCDIR) :

clean :
	@rm -rf $(BOOSTDIR)
	@rm -rf $(SRCDIR)
	@rm -rf $(LIBDIR)
	@rm -rf $(INCDIR)
	@rm -rf $(BINDIR)
	@rm -f configure.log
