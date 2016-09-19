#Variables
CPP = g++
CPPFLAGS = -std=c++11 -Wall -Wextra

DIR_GUARD=@mkdir -p $(@D)

#Target Files
TARGETPATH=../Linux/bin
TARGET = $(TARGETPATH)/se

#Source Files
SOURCES = $(wildcard *.cpp)
S_SOURCES += $(wildcard *.s)

#Man pages
MAN_PAGES=../Documents/se.man

#Object Files
ALLOBJECTS = $(SOURCES:.cpp=.o)
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS += $(S_SOURCES)

#Dependency Files
DEPFILES = $(SOURCES:.cpp=.d)

#Include directories
INCLUDES = -I. -I../ThirdParty -I../ThirdParty/lz4 -I../ThirdParty/bzip2

#Libraries
THIRDPARTY_LIBS = -llz4 -lbz2
LIBS = -lrt -lpthread

#Main target
$(TARGET): $(OBJECTS) $(DEPFILES)
	$(DIR_GUARD)
	cp $(MAN_PAGES) $(TARGETPATH)
	$(CPP) $(OBJECTS) -L../ThirdParty/bzip2 -L../ThirdParty/lz4 $(LIBS) -o $(TARGET)

#To obtain object files
%.o: %.cpp %.d
	$(CPP) -c $(CPPFLAGS) $(INCLUDES)  $< -o $@

#To generate dependency files
%.d: %.cpp
	$(CPP) $(INCLUDES) -MM $< > $@

-include $(DEPFILES)

#To remove generated files
clean:
	rm -f $(TARGET) $(ALLOBJECTS) *.d

#To print 
print-% : ; @echo $* = $($*)
