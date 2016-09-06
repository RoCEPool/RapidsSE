#Variables
CPP = g++
CPPFLAGS = -std=c++11 -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter

#Target Files
TARGET = se.exe
TARGETPATH=bin

#Source Files
SOURCES = $(wildcard *.cpp)
S_SOURCES += $(wildcard *.s)

#Object Files
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS += $(S_SOURCES)

#Dependency Files
DEPFILES = $(SOURCES:.cpp=.d)
#DEPFILES = $(SOURCES:.cpp=.d)

#Include directories
INCLUDES = -I. -I../ThirdParty -I../ThirdParty/lz4 -I../ThirdParty/bzip2

#Libraries
THIRDPARTY_LIBS = -llz4 -lbz2
LIBS = -lrt -lpthread

#Main target
$(TARGET): $(OBJECTS) $(DEPFILES)
	$(CPP) $(OBJECTS) -L../ThirdParty/bzip2 -L../ThirdParty/lz4 $(LIBS) -o $(TARGETPATH)/$(TARGET)

#To obtain object files
%.o: %.cpp %.d
	$(CPP) -c $(CPPFLAGS) $(INCLUDES)  $< -o $@

#To generate dependency files
%.d: %.cpp
	$(CPP) $(INCLUDES) -MM $< > $@

-include $(DEPFILES)

#To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)

#To print 
print-% : ; @echo $* = $($*)
