CC = g++
EXECFILE = test
SOURCE = ../HTTPMessageParser.cpp \
	HTTPMessageParser_test.cpp \
	../ft_istchar.cpp \
	../ft_isspase_and_htab.cpp \
	../ft_isvchar.cpp \
	../ft_trim.cpp

.PHONY: all clean

all: $(EXECFILE)

$(EXECFILE): $(SOURCE)
	g++ $(SOURCE) -o $(EXECFILE) -g -std=c++11 -L/usr/local/lib -pthread -lgtest -lgtest_main

clean:
	rm -rf *.o $(EXECFILE)