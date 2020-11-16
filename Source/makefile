# Makefile for building RapidsSE source code.

# Explicitly define the shell to be used for this makefile. 
SHELL := /bin/bash

# Let's work out what OS we are on and include any platform-specific
# variables and rules.
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
  include makefile.linux.in
else ifeq($(UNAME_S),DARWIN)
  include makefile.darwin.in
else
  $(error This makefile does not handle this operating system: $(UNAME_S))
endif


#Variables
CURRENT_MAKEFILE := $(lastword $(MAKEFILE_LIST))
CPP = g++
CPPFLAGS = -std=c++11 -DWITH_MUNGED_SEN -Wall -Wextra -O3 $(LINUX_CPPFLAGS) $(MACOS_CPPFLAGS)
LIBFLAGS = -shared
PICFLAGS = -fPIC
DIR_GUARD=@mkdir -p $(@D)

#Target Files
CURRENT_PATH=${PWD}
TARGET_PATH = $(strip $(LINUX_TARGET_PATH) $(DARWIN_TARGET_PATH))
TARGET_EXE = $(TARGET_PATH)/rapidsse
TARGET_LIBRARY = $(TARGET_PATH)/libRapidsSE.$(strip $(LINUX_LIBRARY_EXT) $(DARWIN_LIBRARY_EXT))
SHOCO = shoco.o

#Source Files
SOURCES = $(wildcard *.cpp)
SOURCES += $(wildcard ../Munge/*.cpp)
SOURCES += $(wildcard ../ThirdParty/xxhash/*.cpp)
SOURCES += $(wildcard ../ThirdParty/mersenne/*.cpp)
SOURCES += $(wildcard ../ThirdParty/antlr/generated/*.cpp)
SOURCES := $(filter-out main.cpp, $(SOURCES))
S_SOURCES = $(wildcard *.s)

#Man pages
MAN_PAGES=../Documents/se.man

#Object Files for library
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS += $(S_SOURCES:.s=.o)

#Dependency Files for library
DEPFILES = $(SOURCES:.cpp=.d)

#Include directories
INCLUDES = -I. -I../ThirdParty -I../ThirdParty/lz4 -I../ThirdParty/bzip2 -I ../ThirdParty/xxhash -I ../ThirdParty/shoco -I../ThirdParty/mersenne -I../ThirdParty/antlr/include -I../ThirdParty/antlr/generated -I ../ThirdParty/tcc/include -I ../ThirdParty/tcc/src

#Libraries
LIBS = -lpthread -ltcc -ldl -lantlr4-runtime $(strip $(LINUX_LIBS) $(DARWIN_LIBS))
RAPIDS_LIBS = -lRapidsSE
LIB_DIRS = -L../ThirdParty/bzip2 -L../ThirdParty/lz4 -L$(TARGET_PATH) $(strip $(LINUX_LIB_DIRS) $(DARWIN_LIB_DIRS))

# The default target
all : post-build

post-build : main-build

main-build : pre-build
	@$(MAKE) -e --no-print-directory target

pre-build : version

target : $(SHOCO) $(TARGET_EXE) $(TARGET_LIBRARY)

# Debug builds
debug : export CPPFLAGS = -std=c++11 -DWITH_DEBUG_VERSION -DWITH_MUNGED_SEN -Wall -Wno-unused -g $(strip $(LINUX_CPPFLAGS_DEBUG) $(DARWIN_CPPFLAGS_DEBUG))
debug : all

# Explicit release builds (the default)
release : all

$(SHOCO): ../ThirdParty/shoco/shoco.c  
	$(CPP) -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -O3 -Wall -Wno-narrowing -c $(INCLUDES) ../ThirdParty/shoco/shoco.c 

#Library target
$(TARGET_LIBRARY): $(OBJECTS) $(DEPFILES)
	$(DIR_GUARD)
	$(CPP) $(LIBFLAGS) $(LIB_DIRS) $(LIBS) -o $(TARGET_LIBRARY) $(OBJECTS)

#Main target
$(TARGET_EXE): $(TARGET_LIBRARY) $(SHOCO)
	$(DIR_GUARD)
	$(CPP) -c $(CPPFLAGS) $(INCLUDES)  main.cpp -o main.o
	cp $(MAN_PAGES) $(TARGET_PATH)
	$(CPP) main.o $(SHOCO) $(LIB_DIRS) $(RAPIDS_LIBS) $(LIBS) -o $(TARGET_EXE)

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

# Necessary to make the version rule above run every time.
FORCE : 

#To remove generated files
clean:
	rm -f $(TARGET_EXE) $(TARGET_LIBRARY) $(OBJECTS) *.d

#To print 
print-% : ; @echo $* = $($*)
