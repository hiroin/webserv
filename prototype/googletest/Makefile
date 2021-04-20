CC = g++
EXECFILE = test
SOURCE = ../HTTPMessageParser.cpp \
	HTTPMessageParser_test.cpp \
	../ft_istchar.cpp \
	../ft_isspase_and_htab.cpp \
	../ft_isvchar.cpp \
	../ft_trim.cpp \
	ft_split_use_find_first_of_test.cpp \
	../ft_split_use_find_first_of.cpp

.PHONY: all clean

all: $(EXECFILE)

$(EXECFILE): $(SOURCE)
	g++ $(SOURCE) -o $(EXECFILE) -g -std=c++11 -L/usr/local/lib -pthread -lgtest -lgtest_main

ft: test_ft
test_ft: ft_split_use_find_first_of_test.cpp ../ft_split_use_find_first_of.cpp
	g++ ft_split_use_find_first_of_test.cpp ../ft_split_use_find_first_of.cpp -o test_ft -g -std=c++11 -L/usr/local/lib -pthread -lgtest -lgtest_main

clean:
	rm -rf *.o $(EXECFILE) test_ft