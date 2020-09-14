#Variables
CPP = g++
CPPFLAGS = -std=c++11 -DWITH_LINUX -DWITH_MUNGED_SEN -Wall -Wextra -O3
#CPPFLAGS = -std=c++11 -DWITH_DEBUG_VERSION -DWITH_LINUX -DWITH_MUNGED_SEN -Wall -Wno-unused -g
LIBFLAGS = -shared
PICFLAGS = -fPIC
DIR_GUARD=@mkdir -p $(@D)

#Target Files
CURRENT=${PWD}
TARGET_PATH = $(CURRENT)/../Linux/bin
TARGET_EXE = $(TARGET_PATH)/rapidsse
TARGET_LIBRARY = $(TARGET_PATH)/libRapidsSEv0.4.1.so
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
INCLUDES = -I. -I../ThirdParty -I../ThirdParty/lz4 -I../ThirdParty/bzip2 -I ../ThirdParty/xxhash -I ../ThirdParty/shoco -I../ThirdParty/mersenne -I../ThirdParty/antlr/include -I../ThirdParty/antlr/generated

#Libraries
THIRDPARTY_LIBS = -llz4 -lbz2 -lantlr4-runtime
LIBS = -lrt -lpthread -ltcc -ldl
RAPIDS_LIBS = -lRapidsSEv0.4.1
LIB_DIRS = -L../ThirdParty/bzip2 -L../ThirdParty/lz4 -L/usr/local/lib -L../ThirdParty/antlr/linux/centos7  -L$(TARGET_PATH)

#all : $(SHOCO) $(TARGET_EXE) $(TARGET_LIBRARY)
all : post-build

post-build : main-build
	
main-build : pre-build
	@$(MAKE) -e --no-print-directory target

pre-build : version

target : $(SHOCO) $(TARGET_EXE) $(TARGET_LIBRARY)

#Debug builds
debug : export CPPFLAGS = -std=c++11 -DWITH_DEBUG_VERSION -DWITH_LINUX -DWITH_MUNGED_SEN -Wall -Wno-unused -g
debug : all
#debug: $(SHOCO) $(TARGET_EXE) $(TARGET_LIBRARY)

$(SHOCO): ../ThirdParty/shoco/shoco.c  
	$(CPP) -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -O3 -Wall -c $(INCLUDES) ../ThirdParty/shoco/shoco.c 

#Library target
$(TARGET_LIBRARY): $(OBJECTS) $(DEPFILES)
	$(DIR_GUARD)
	$(CPP) $(LIBFLAGS) $(LIBS) -o $(TARGET_LIBRARY) $(OBJECTS)

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
version : FORCE
	./generateVersion.sh

# Necessary to make the version rule above run every time.
FORCE : 

#To remove generated files
clean:
	rm -f $(TARGET_EXE) $(TARGET_LIBRARY) $(OBJECTS) *.d

#To print 
print-% : ; @echo $* = $($*)
