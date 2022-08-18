SHELL   = /bin/sh

TARGET  = main
SOURCES = $(shell echo *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
LIBRARY = $(OBJECTS:.o=.so)
#~ CFLAGS += $(shell pkg-config --cflags --libs  geany)
CFLAGS += -W
CFLAGS += -Wall
CFLAGS += -O2
#~ CFLAGS += -lX11 -lXrandr

$(LIBRARY): $(SOURCES)
	g++  $(CFLAGS) -c $(SOURCES)
	g++ -o $(TARGET).exe $(OBJECTS)
#~ 	g++ -shared -o $(LIBRARY) $(OBJECTS);


install:
	echo "installing"

#uninstall:
	#@rm -f $(BINDIR)/$(LIBRARY)

.PHONY : clean
clean :
	#@rm -f $(OBJECTS)
	#@rm -f $(LIBRARY)
