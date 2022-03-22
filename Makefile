CC=g++
#OPT=-O1
OPT=-O3
CXXFLAGS=-Wall
LIBS=-lpthread

CXXFILES=main.cpp lib/easywsclient.cpp
OBJECTS=main.o easywsclient.o

BINARY=bin/ws

dir_guard=@mkdir -p bin

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(dir_guard)
	$(CC) $^ $(LIBS) $(OPT) $(CXXFLAGS) -o $@ 

%.o:%.cpp
	$(CC) $(CXXFLAGS) $(OPT) -c $^

%.o:lib/%.cpp
	$(CC) $(CXXFLAGS) $(OPT) -c $^

clean:
	rm -rf $(BINARY) $(OBJECTS)

move:
	mv bin/ws ~/bin/
