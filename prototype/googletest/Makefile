CC = g++
EXECFILE = test
SOURCE = ../HTTPMessageParser.cpp \
	../ft_istchar.cpp \
	../ft_isspase_and_htab.cpp \
	../ft_isvchar.cpp \
	../ft_trim.cpp \
	../ft_split_use_find_first_of.cpp \
	../ft_isunreserved.cpp \
	../ft_issubdelims.cpp \
	../ft_stoi.cpp \
	../Client.cpp \
	../recvData.cpp \
	../ft_memset.cpp

.PHONY: all clean

hmp: $(SOURCE) HTTPMessageParser_test.cpp
	g++ $(SOURCE) HTTPMessageParser_test.cpp -o $(EXECFILE) -g -std=c++11 -L/usr/local/lib -pthread -lgtest -lgtest_main

ft: $(SOURCE) ft_split_use_find_first_of_test.cpp
	g++ ft_split_use_find_first_of_test.cpp ../ft_split_use_find_first_of.cpp ft_stoi_test.cpp ../ft_stoi.cpp -o $(EXECFILE) -g -std=c++11 -L/usr/local/lib -pthread -lgtest -lgtest_main

client: $(SOURCE) Client_test.cpp
	g++ $(SOURCE) Client_test.cpp  -o $(EXECFILE) -g -std=c++11 -L/usr/local/lib -pthread -lgtest -lgtest_main


clean:
	rm -rf *.o $(EXECFILE) test_ft