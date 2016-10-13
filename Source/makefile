#Variables
CPP = g++
CPPFLAGS = -std=c++11 -Wall -Wextra -O3
AR = ar
ARFLAGS = rs

DIR_GUARD=@mkdir -p $(@D)

#Target Files
TARGETPATH=../Linux/bin
TARGET_EXE = $(TARGETPATH)/se 
TARGET_LIBRARY = $(TARGETPATH)/libRapidsSE.a

#Source Files
SOURCES = $(wildcard *.cpp)
SOURCES := $(filter-out main.cpp, $(SOURCES))
S_SOURCES = $(wildcard *.s)

#Man pages
MAN_PAGES=../Documents/se.man

#Object Files for library
ALLOBJECTS = $(SOURCES:.cpp=.o)
OBJECTS = $(SOURCES:.cpp=.o)
S_OBJECTS = $(S_SOURCES:.s=.o)

#Dependency Files for library
DEPFILES = $(SOURCES:.cpp=.d)

#Include directories
INCLUDES = -I. -I../ThirdParty -I../ThirdParty/lz4 -I../ThirdParty/bzip2

#Libraries
THIRDPARTY_LIBS = -llz4 -lbz2
LIBS = -lrt -lpthread

all : $(TARGET_EXE) $(TARGET_LIBRARY)

#Library target
$(TARGET_LIBRARY): $(OBJECTS) $(S_OBJECTS) $(DEPFILES)
	$(DIR_GUARD)
	$(AR) $(ARFLAGS) $(TARGET_LIBRARY) $(OBJECTS) $(S_OBJECTS)

#Main target
$(TARGET_EXE): main.o $(TARGET_LIBRARY)
	$(DIR_GUARD)
	cp $(MAN_PAGES) $(TARGETPATH)
	$(CPP) main.o -L../ThirdParty/bzip2 -L../ThirdParty/lz4 $(LIBS) $(TARGET_LIBRARY) -o $(TARGET_EXE)

#To obtain object files
%.o: %.cpp %.d
	$(CPP) -c $(CPPFLAGS) $(INCLUDES)  $< -o $@

%.o: %.s
	$(CPP) -c $(CPPFLAGS) $(INCLUDES)  $< -o $@

#To generate dependency files
%.d: %.cpp
	$(CPP) $(INCLUDES) -MM $< > $@

-include $(DEPFILES)

#To remove generated files
clean:
	rm -f $(TARGET_EXE) $(TARGET_LIBRARY) $(ALLOBJECTS) *.d

#To print 
print-% : ; @echo $* = $($*)
