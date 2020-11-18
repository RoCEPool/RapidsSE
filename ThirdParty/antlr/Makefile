# Custom makefile to only build ANTLR if the library has not yet been compiled.

# Explicitly define the shell to be used for this makefile. 
SHELL := /bin/bash

CURRENT_PATH = ${PWD}
LIBDIR := $(CURRENT_PATH)/lib
SRCDIR := $(CURRENT_PATH)/src
INCDIR := $(CURRENT_PATH)/include

LIBANTLR := libantlr4-runtime.a
SRCZIP := $(CURRENT_PATH)/antlr4-cpp-runtime-4.8-source.zip

# Let's work out what OS we are on 
UNAME_S := $(shell uname -s)

## Check for presence of cmake
CMAKE_EXE := $(shell which cmake)
ifndef CMAKE_EXE
  define ERR
cmake is not installed and is required to build ANTLR. Please install it with:
  on Linux:  sudo yum install cmake
  on MacOS:  brew install cmake

  endef
  $(error $(ERR))
endif

# Check for presence of gcc >= 5 if we are on Linux
ifeq ($(UNAME_S),Linux)
  GCC_VERSION_GTE5 := $(shell echo `gcc -dumpversion | cut -f1-2 -d.` \>= 5 | bc)
  ifeq ($(GCC_VERSION_GTE5), 0)
    define ERR
gcc version 5 or later is required. You have v$(shell gcc -dumpversion).

To install gcc 5 on Centos7 run the following commands:
  sudo yum install libuuid-devel centos-release-scl devtoolset-7-gcc*

Then from the Source directory run the following command to build rapidsse:
  scl enable devtoolset-7 'make -j 8'

    endef
    $(error $(ERR))
  endif
endif

all : $(LIBDIR)/$(LIBANTLR) $(INCDIR)

$(LIBDIR)/$(LIBANTLR) :
	@mkdir -p $(LIBDIR)
	@mkdir -p $(SRCDIR)
	@unzip -o $(SRCZIP) -d $(SRCDIR)
	cd $(SRCDIR) && cmake -DCMAKE_BUILD_TYPE=Release CMakeLists.txt
	+cd $(SRCDIR) && $(MAKE) -e --no-print-directory 
	cp $(SRCDIR)/dist/$(LIBANTLR) $(LIBDIR)
	cd $(SRCDIR)/runtime/src && find . -name '*.h' | cpio -pdm $(INCDIR)
	@rm -rf $(SRCDIR)

# Empty rules
$(INCDIR) :

clean :
	@rm -rf $(SRCDIR) 
	@rm -f  $(LIBDIR)/*.a 
	@rm -rf $(INCDIR)
