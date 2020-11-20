# Makefile for building RapidsSE source code.

# Explicitly define the shell to be used for this makefile. 
SHELL := /bin/bash

# Get the directory of this makefile
#  Ref : https://stackoverflow.com/questions/18136918/how-to-get-current-relative-directory-of-your-makefile
CURRENT_MAKEFILE := $(lastword $(MAKEFILE_LIST))
MAKEFILE_PATH := $(abspath $(CURRENT_MAKEFILE))
CURRENT_PATH := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

# Let's work out what OS we are on and include any platform-specific
# variables and rules.
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
  include makefile.linux.in
else ifeq ($(UNAME_S),Darwin)
  include makefile.darwin.in
else
  $(error This makefile does not handle this operating system: $(UNAME_S))
endif


# Variables
CPP = g++
CPPFLAGS = -std=c++11 -DWITH_MUNGED_SEN -Wall -Wextra -O3 $(LINUX_CPPFLAGS) $(MACOS_CPPFLAGS)
LIBFLAGS = -shared
PICFLAGS = -fPIC
DIR_GUARD=@mkdir -p $(@D)

# Target Files
TARGET_PATH = ../bin
TARGET_EXE = $(TARGET_PATH)/rapidsse
TARGET_LIBRARY = $(TARGET_PATH)/libRapidsSE.$(strip $(LINUX_LIBRARY_EXT) $(DARWIN_LIBRARY_EXT))
SHOCO := shoco.o

# Source Files
SOURCES := $(shell find . -name "*.cpp")
SOURCES += $(wildcard ../Munge/*.cpp)
SOURCES += $(wildcard ../ThirdParty/xxhash/*.cpp)
SOURCES += $(wildcard ../ThirdParty/mersenne/*.cpp)
SOURCES += $(wildcard ../ThirdParty/antlr/generated/*.cpp)
SOURCES := $(filter-out ./main/main.cpp, $(SOURCES))
S_SOURCES := $(shell find . -name "*.s")

# Man pages
MAN_PAGES=../Documents/se.man

# Object Files for library
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS += $(S_SOURCES:.s=.o)

# Dependency Files for library
DEPFILES = $(SOURCES:.cpp=.d)

# Include directories
SRC_INCLUDES := $(shell find . -type d | awk '{print "-I"$$0}')
THIRDPARTY_INCLUDES := -I../ThirdParty -I../ThirdParty/lz4 -I../ThirdParty/bzip2 -I ../ThirdParty/xxhash -I ../ThirdParty/shoco -I../ThirdParty/mersenne -I../ThirdParty/antlr/include -I../ThirdParty/antlr/generated -I ../ThirdParty/tcc/include -I ../ThirdParty/tcc/src
INCLUDES = $(SRC_INCLUDES) $(THIRDPARTY_INCLUDES)

# Linker libraries
LIBS = -lpthread -ltcc -ldl -lantlr4-runtime $(strip $(LINUX_LIBS) $(DARWIN_LIBS))
RAPIDS_LIBS = -lRapidsSE
LIB_DIRS = -L../ThirdParty/bzip2 -L../ThirdParty/lz4 -L$(TARGET_PATH) -L../ThirdParty/tcc/lib -L../ThirdParty/antlr/lib $(strip $(LINUX_LIB_DIRS) $(DARWIN_LIB_DIRS))

# ThirdParty compiled library targets
ANTLR_LIB := ../ThirdParty/antlr/lib/libantlr4-runtime.a
TCC_LIB := ../ThirdParty/tcc/lib/libtcc.a
COMPILED_LIBS := $(ANTLR_LIB) $(TCC_LIB)

###############################################################################
### Build Rules																														  ###
###############################################################################

# The default target (because it is first)
all : post-build

post-build : main-build

main-build : pre-build
	@$(MAKE) -e --no-print-directory target

pre-build : version thirdparty

target : $(SHOCO) $(TARGET_EXE) $(TARGET_LIBRARY)

# Debug builds
Debug : debug
debug : export CPPFLAGS = -std=c++11 -DWITH_DEBUG_VERSION -DWITH_MUNGED_SEN -Wall -Wno-unused -g $(strip $(LINUX_CPPFLAGS_DEBUG) $(DARWIN_CPPFLAGS_DEBUG))
debug : all

# Explicit release builds (the default)
Release : release
release : all

$(SHOCO): ../ThirdParty/shoco/shoco.c  
	$(CPP) -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -O3 -Wall -Wno-narrowing -c $(INCLUDES) ../ThirdParty/shoco/shoco.c 

#Library target
$(TARGET_LIBRARY): $(OBJECTS) $(DEPFILES) $(COMPILED_LIBS)
	$(DIR_GUARD)
	$(CPP) $(LIBFLAGS) $(LIB_DIRS) $(LIBS) -o $(TARGET_LIBRARY) $(OBJECTS)

#Main target
$(TARGET_EXE): $(TARGET_LIBRARY) $(SHOCO) $(COMPILED_LIBS)
	$(DIR_GUARD)
	$(CPP) -c $(CPPFLAGS) $(INCLUDES)  ./main/main.cpp -o ./main/main.o
	cp $(MAN_PAGES) $(TARGET_PATH)
	$(CPP) ./main/main.o $(SHOCO) $(LIB_DIRS) $(RAPIDS_LIBS) $(LIBS) -o $(TARGET_EXE)

#To obtain object files
%.o: %.cpp %.d
	$(CPP) -c $(CPPFLAGS) $(PICFLAGS) $(INCLUDES)  $< -o $@

%.o: %.s %.d
	$(CPP) -c $(CPPFLAGS) $(PICFLAGS) $(INCLUDES)  $< -o $@

#To generate dependency files
%.d: %.cpp 
	$(CPP) $(CPPFLAGS) $(INCLUDES) -MM $< > $@

# Generate the Version.h file. Force it to run every time.
# The script will determine if it needs to update the Version.h file.
version : FORCE
	@./generateVersion.sh

# Generic rule to make any thirdparty libraries that are built from source
# and then linked against by RapidsSE. Add the dependencies here and then
# add a corresponding rule with a recipe for how to build it.
thirdparty : $(COMPILED_LIBS)

# Rule for building ANTLR
$(ANTLR_LIB) : 
	+@cd ../ThirdParty/antlr && $(MAKE)

# Rule for building TCC
$(TCC_LIB) : 
	+@cd ../ThirdParty/tcc && $(MAKE)

# Necessary to make some of the above rules run every time.
FORCE : 


# Removes all generated files, including RapidsSE and 3rd-party compiled libs
distclean : clean
	@echo ""
	@echo "Removing 3rd-party generated libraries..."
	@echo ""
	+@cd ../ThirdParty/antlr && $(MAKE) clean
	+@cd ../ThirdParty/tcc && $(MAKE) clean

#To remove RapidsSE-generated files only
clean :
	rm -f $(TARGET_EXE) $(TARGET_LIBRARY) $(OBJECTS) *.d

# Makefile debugging rule to print makefile variables.
# e.g. make print-ANTLR_LIB
print-% : ; @echo $* = $($*)
