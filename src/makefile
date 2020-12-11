# Makefile for building RapidsSE source code.
#
# A quick primer on some syntax:
#   A '+' before a command causes that command to execute using the
#   job controller of the current make instance. e.g. so sub-makes
#   conform to the maximum number of concurrent jobs.
#
#   A '-' before a command indicates that make should ignore the
#   exit status that the command returns.
#
#   A '@' before a command means that the command will not be echoed
#   to stdout. Any output from the command will appear, but not the
#   command invocation itself.
#   
#   Within a rule:
#     '$@' refers to the filename of the target of the rule.
#     '$<' refers to the first prerequisite of that rule.
#     '$?' refers to the space-separated name of all prerequisites of
#          the rule that are newer than the rule target.
#     '$^' refers to the space-separated name of all prerequisites.

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

CPPFLAGS = -std=c++11 -Wall -Wextra -O3 $(strip $(LINUX_CPPFLAGS) $(MACOS_CPPFLAGS))
CPPFLAGS_DEBUG = -std=c++11 -DWITH_DEBUG_VERSION -Wall -Wno-unused -O0 -g3 $(strip $(LINUX_CPPFLAGS_DEBUG) $(DARWIN_CPPFLAGS_DEBUG))

LDFLAGS = -shared -O3 $(strip $(LINUX_LDFLAGS) $(DARWIN_LDFLAGS))
LDFLAGS_DEBUG = -shared -Og -g3 $(strip $(LINUX_LDFLAGS_DEBUG) $(DARWIN_LDFLAGS_DEBUG))

PICFLAGS = -fPIC
DIR_GUARD=@mkdir -p $(@D)

# Target Files
TARGET_PATH = ../bin
TARGET_EXE = $(TARGET_PATH)/rapidsse
TARGET_LIBRARY = $(TARGET_PATH)/libRapidsSE.$(strip $(LINUX_LIBRARY_EXT) $(DARWIN_LIBRARY_EXT))

# Man pages
MAN_PAGES=../Documents/se.man

# Source Files
SRC_DIR := .
MAIN_CPP := $(shell find $(SRC_DIR) -name "main.cpp")
SHOCO_C := ../thirdparty/shoco/shoco.c  # Not currently used
SOURCES := $(shell find $(SRC_DIR) -name "*.cpp")
SOURCES += $(wildcard $(SRC_DIR)/../munge/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/../thirdparty/xxhash/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/../thirdparty/mersenne/*.cpp)

SQL_GRAMMAR_CPP := $(wildcard $(SRC_DIR)/../thirdparty/antlr/generated/sql*.cpp)
JSON5_GRAMMAR_CPP := $(wildcard $(SRC_DIR)/../thirdparty/antlr/generated/json5*.cpp)
SOURCES += $(SQL_GRAMMAR_CPP)
SOURCES += $(JSON5_GRAMMAR_CPP)

SOURCES := $(filter-out $(MAIN_CPP), $(SOURCES))
S_SOURCES := $(shell find $(SRC_DIR) -name "*.s")

SQL_GRAMMAR := ../thirdparty/antlr/sql.g4
JSON5_GRAMMAR := ../thirdparty/antlr/json5.g4

# Object directories
# We separate debug & release so that swapping between builds doesn't mean
# a complete rebuild of all object files.
BASE_OBJ_DIR := ../build/obj
DEBUG_OBJ_DIR := $(BASE_OBJ_DIR)/debug/src
RELEASE_OBJ_DIR := $(BASE_OBJ_DIR)/release/src

# Where do we put our object files? Set the OBJ_DIR variable appropriately.
ifndef BUILD_CONFIG
  ifeq ($(MAKECMDGOALS),debug)
    BUILD_CONFIG := debug
    OBJ_DIR := $(DEBUG_OBJ_DIR)

  else ifeq ($(MAKECMDGOALS),release)
    BUILD_CONFIG := release
    OBJ_DIR := $(RELEASE_OBJ_DIR)

  else
    # All other goals (all, clean, etc).
    BUILD_CONFIG := release
    OBJ_DIR := $(RELEASE_OBJ_DIR)
  endif

  # Make BUILD_CONFIG and OBJ_DIR available to sub-makes.
  export BUILD_CONFIG
  export OBJ_DIR
endif

# Object Files for library
OBJECTS := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
OBJECTS += $(patsubst $(SRC_DIR)/%.s,   $(OBJ_DIR)/%.obj, $(S_SOURCES))
MAIN_O  := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(MAIN_CPP))
SHOCO_O := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SHOCO_C)) 

# Dependency Files for library
DEPFILES  = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.d, $(SOURCES))
DEPFILES += $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.d, $(MAIN_CPP))

# Include directories
SRC_INCLUDES := $(shell find . -type d | awk '{print "-I"$$0}')
THIRDPARTY_INCLUDES := -I../thirdparty -I../thirdparty/lz4 -I../thirdparty/bzip2 -I ../thirdparty/xxhash -I ../thirdparty/shoco -I../thirdparty/mersenne -I../thirdparty/antlr/include -I../thirdparty/antlr/generated -I ../thirdparty/tcc/include -I ../thirdparty/tcc/src
INCLUDES = $(SRC_INCLUDES) $(THIRDPARTY_INCLUDES)

# Linker libraries
LIBS = -lpthread -ltcc -ldl -lantlr4-runtime $(strip $(LINUX_LIBS) $(DARWIN_LIBS))
RAPIDS_LIBS = -lRapidsSE
LIB_DIRS = -L../thirdparty/bzip2 -L../thirdparty/lz4 -L$(TARGET_PATH) -L../thirdparty/tcc/lib -L../thirdparty/antlr/lib $(strip $(LINUX_LIB_DIRS) $(DARWIN_LIB_DIRS))

# ThirdParty compiled library targets
ANTLR_LIB := ../thirdparty/antlr/lib/libantlr4-runtime.a
TCC_LIB := ../thirdparty/tcc/lib/libtcc.a
COMPILED_LIBS := $(ANTLR_LIB) $(TCC_LIB)

# A callable function for defining the file that describes whether a target
# was built as debug or release.
targetConfigFile = $(shell echo -n "$$(dirname $1)/.$$(basename $1).lastBuildConfig")



###############################################################################
### Build Rules																														  ###
###############################################################################

# The default target (because it is first)
all : post-build

post-build : main-build

main-build : pre-build
	@$(MAKE) -e target

pre-build : version thirdparty grammar buildConfigCheck

target : $(TARGET_EXE) $(TARGET_LIBRARY)

# Debug builds
debug : export CPPFLAGS = $(CPPFLAGS_DEBUG)
debug : export LDFLAGS = $(LDFLAGS_DEBUG)
debug : all

# Explicit release builds (the default)
release : all

#$(SHOCO_O): $(SHOCO_C)
#	$(DIR_GUARD)
#	$(CPP) -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -O3 -Wall -Wno-narrowing -c $(INCLUDES) $< -o $@ 

# RapidsSE dynamic library
$(TARGET_LIBRARY): $(OBJECTS) $(DEPFILES) $(COMPILED_LIBS)
	$(DIR_GUARD)
	@echo ""
	@echo "Linking the rapidsse dynamic library..."
	@echo ""
	$(CPP) $(LDFLAGS) $(LIB_DIRS) $(LIBS) -o $(TARGET_LIBRARY) $(OBJECTS)
	@echo "$(BUILD_CONFIG)" > $(call targetConfigFile, $(TARGET_LIBRARY))

# RapidsSE executable
$(TARGET_EXE): $(TARGET_LIBRARY) $(MAIN_O) $(COMPILED_LIBS)
	$(DIR_GUARD)
	@echo ""
	cp $(MAN_PAGES) $(TARGET_PATH)
	@echo ""
	@echo "Linking the rapidsse executable..."
	@echo ""
	$(CPP) $(MAIN_O) $(LIB_DIRS) $(RAPIDS_LIBS) $(LIBS) -o $(TARGET_EXE)
	@echo "$(BUILD_CONFIG)" > $(call targetConfigFile, $(TARGET_EXE))

# Compile source .cpp files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR)/%.d
	$(DIR_GUARD)
	$(CPP) -c $(CPPFLAGS) $(PICFLAGS) $(INCLUDES)  $< -o $@

# Compile assembly files into objects
$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.s 
	$(DIR_GUARD)
	$(CPP) -c $(CPPFLAGS) $(PICFLAGS) $(INCLUDES)  $< -o $@

# Include all of the generated dependency rules from all .d files.
# The '-' means ignore the exit status of the command.
ifeq (1, $(words $(findstring $(MAKECMDGOALS), target)))
  # Only include these dependency files when we are building 'target'
  # otherwise a make clean somehow ends up generating the dependency files.
  -include $(DEPFILES)
endif

# The rule to generate dependency files (*.d) for cpp files from the source directory.
# The sed script in here is to make the dependency file (*.d) itself dependent
# on changes to the source files. So if a source file is changed then the dependency
# files that depend on that source need to be regenerated, which in turn will affect
# other source files.
#
# e.g., if I change a header file then everything that depends on that header also
# needs to be recompiled. We can fulfil this by making the dependency file depend
# on the header too.
#
# e.g. foo.cpp depends on foo.hpp. But bar.cpp also depends on foo.hpp. So we generate
# the dependency files foo.d and bar.d that include these rules. But if we now modify 
# foo.hpp to #include baz.hpp then we also need to update the dependency files for
# foo.cpp and bar.cpp so that they now also depend on baz.hpp.
# 
# Documentation on this can be found here:
# https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html

$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp
	$(DIR_GUARD)
	@echo "Generating dependency file: $@"
	@set -e; \
	rm -f $@; \
	$(CPP) $(CPPFLAGS) $(INCLUDES) -MM $< > $@.$$$$; \
	sed 's,^.*\.o[ :]*,$(OBJ_DIR)/$*.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$;
#	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; 

# Generate the Version.h file. Force it to run every time.
# The script will determine if it needs to update the Version.h file.
version : FORCE
	@./generateVersion.sh

# Rules to build the parser/lexers for the various grammar if the .g4 files change
grammar : $(SQL_GRAMMAR_CPP) $(JSON5_GRAMMAR_CPP)

# This is a hack to avoid make from invoking the generate.sh script for each input file
$(firstword $(SQL_GRAMMAR_CPP)) : $(SQL_GRAMMAR)
	@-../thirdparty/antlr/generate.sh $(SQL_GRAMMAR)

$(firstword $(JSON5_GRAMMAR_CPP)) : $(JSON5_GRAMMAR)
	@-../thirdparty/antlr/generate.sh $(JSON5_GRAMMAR)


# Since we write out a debug/release binary in the same location regardless
# of whether it is a debug or release configuration, we need to handle the
# case where someone does a make debug and then a make release. Because the
# binaries already exist the release version won't be rebuilt. So we check
# for the presence of a file that was written out with the binary that tells
# us whether it was written as a debug or release.
buildConfigCheck : FORCE
	@./checkBuildConfig.sh $(TARGET_LIBRARY) $(call targetConfigFile, $(TARGET_LIBRARY)) $(BUILD_CONFIG)
	@./checkBuildConfig.sh $(TARGET_EXE) $(call targetConfigFile, $(TARGET_EXE)) $(BUILD_CONFIG)

# Generic rule to make any thirdparty libraries that are built from source
# and then linked against by RapidsSE. Add the dependencies here and then
# add a corresponding rule with a recipe for how to build it.
thirdparty : $(COMPILED_LIBS)

# Rule for building ANTLR
antlr : $(ANTLR_LIB)
$(ANTLR_LIB) : 
	+@cd ../thirdparty/antlr && env -u CPPFLAGS -u LDFLAGS $(MAKE)

# Rule for building TCC
tcc : $(TCC_LIB)
$(TCC_LIB) : 
	env
	+@cd ../thirdparty/tcc && env -u CPPFLAGS -u LDFLAGS $(MAKE)

# Necessary to make some of the above rules run every time.
FORCE : 


# Removes all generated files, including RapidsSE and 3rd-party compiled libs
distclean : clean
	@echo ""
	@echo "Removing 3rd-party generated libraries..."
	@echo ""
	+@cd ../thirdparty/antlr && (MAKE) clean
	+@cd ../thirdparty/tcc && $(MAKE) clean

#To remove RapidsSE-generated files only
clean :
	rm -f $(TARGET_EXE) 
	rm -f $(TARGET_LIBRARY) 
	rm -f $(call targetConfigFile, $(TARGET_EXE)) 
	rm -f $(call targetConfigFile, $(TARGET_LIBRARY))
	rm -rf $(BASE_OBJ_DIR)

# Makefile debugging rule to print makefile variables.
# e.g. make print-ANTLR_LIB
print-% : ; @echo $* = $($*)

# Don't delete object and dependency files after building. 
# Specifying no targets here means that make will preserve all targets,
# including objects and dependency files.
.SECONDARY :

# List of make targets that don't correlate to files in the filesystem.
.PHONY : clean distclean FORCE thirdparty antlr tcc version sql buildConfigCheck target release debug post-build main-build pre-build all
