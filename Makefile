CC = g++
CFLAGS = -c -std=c++0x

WFLAGS = -Wall 

OPTS = -O0
OFLAGS =  -g2 
EXECUTABLE = test

CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))


all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(WFLAGS) $(OPTS) $(LFLAGS) $^ -o $@   $(LFLAGS)

obj/%.o: src/%.cpp
	mkdir -p ./obj
	$(CC) $(CFLAGS) $(WFLAGS) $(OPTS) $(OFLAGS)  -c -o $@ $<  



clean:
	rm -f $(OBJ_FILES)
	rm -f *.out
	rm -f *~
	rm -f $(EXECUTABLE) 
