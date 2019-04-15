#Variables
CPP = g++
CPPFLAGS = -std=c++11 -D__LINUX__ -DMUNGED -Wall -Wextra -O3
#CPPFLAGS = -std=c++11 -D_DEBUG -D__LINUX__ -DMUNGED -Wall -Wno-unused -g
LIBFLAGS = -shared
PICFLAGS = -fPIC
DIR_GUARD=@mkdir -p $(@D)

#Target Files
CURRENT=${PWD}
TARGET_PATH = $(CURRENT)/../Linux/bin
TARGET_EXE = $(TARGET_PATH)/rapidsse
TARGET_LIBRARY = $(TARGET_PATH)/libRapidsSEv0.4.1.so

#Source Files
SOURCES = $(wildcard *.cpp)
SOURCES += $(wildcard ../Munge/*.cpp)
SOURCES += $(wildcard ../ThirdParty/xxhash/*.cpp)
SOURCES += $(../ThirdParty/shoco/*.c)
SOURCES := $(filter-out main.cpp, $(SOURCES))
#S_SOURCES = $(wildcard *.s)

#Man pages
MAN_PAGES=../Documents/se.man

#Object Files for library
OBJECTS = $(SOURCES:.cpp=.o)
#OBJECTS += $(S_SOURCES:.s=.o)

#Dependency Files for library
DEPFILES = $(SOURCES:.cpp=.d)

#Include directories
INCLUDES = -I. -I../ThirdParty -I../ThirdParty/lz4 -I../ThirdParty/bzip2 -I ../ThirdParty/xxhash -I ../ThirdParty/shoco

#Libraries
THIRDPARTY_LIBS = -llz4 -lbz2
LIBS = -lrt -lpthread
RAPIDS_LIBS = -lRapidsSEv0.4.1

all : $(TARGET_EXE) $(TARGET_LIBRARY)

#Library target
$(TARGET_LIBRARY): $(OBJECTS) $(DEPFILES)
	$(DIR_GUARD)
	$(CPP) $(LIBFLAGS) $(LIBS) -o $(TARGET_LIBRARY) $(OBJECTS)

#Main target
$(TARGET_EXE): $(TARGET_LIBRARY)
	$(DIR_GUARD)
	$(CPP) -c $(CPPFLAGS) $(INCLUDES)  main.cpp -o main.o
	cp $(MAN_PAGES) $(TARGET_PATH)
	$(CPP) main.o -L../ThirdParty/bzip2 -L../ThirdParty/lz4 -L$(TARGET_PATH) $(LIBS) $(RAPIDS_LIBS) -o $(TARGET_EXE)

#To obtain object files
%.o: %.cpp %.d
	$(CPP) -c $(CPPFLAGS) $(PICFLAGS) $(INCLUDES)  $< -o $@

%.o: %.s %.d
	$(CPP) -c $(CPPFLAGS) $(PICFLAGS) $(INCLUDES)  $< -o $@

#To generate dependency files
%.d: %.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDES) -MM $< > $@

-include $(DEPFILES)

#To remove generated files
clean:
	rm -f $(TARGET_EXE) $(TARGET_LIBRARY) $(OBJECTS) *.d

#To print 
print-% : ; @echo $* = $($*)
