CXX=g++

CFLAGS=-Wall
LDFLAGS=-lncurses -lpanel -lutil
BIN=../bin/
INC=../include/

TARGETS=$(shell ls *.cc | sed s/.cc/.o/g)


all: $(TARGETS) link clean

clean:
	@echo "Removing all .o files."
	@rm -f *.o

link:
	$(CXX) $(TARGETS) $(LDFLAGS) -o $(BIN)boothby

.cc.o:
	$(CXX) $(CFLAGS) -I$(INC) -c -o $@ $<
