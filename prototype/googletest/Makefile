CXX = clang++
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
	../ft_istoken.cpp \
	../Client.cpp \
	../recvData.cpp \
	../ft_isctl.cpp \
	../ft_istspecials.cpp \
	../getChunked.cpp \
	../Response.cpp \
  ../Config.cpp \
	../parseConfig.cpp \
	../readData.cpp \
	../php-cgi.cpp \
	../normal-cgi.cpp \
	../cgiTester.cpp \
	../POST_normal-cgi.cpp \
	../POST_phpCGI.cpp \
	../POST_cgiTester.cpp
OBJS = $(SOURCE:.cpp=.o)

.PHONY: all clean

hmp: $(SOURCE) HTTPMessageParser_test.cpp
	clang++ $(SOURCE) HTTPMessageParser_test.cpp -o $(EXECFILE) -g -std=c++11 -L/usr/local/lib -pthread -lgtest -lgtest_main

ft: $(SOURCE) ft_split_use_find_first_of_test.cpp
	clang++ ft_split_use_find_first_of_test.cpp ../ft_split_use_find_first_of.cpp ft_stoi_test.cpp ../ft_stoi.cpp -o $(EXECFILE) -g -std=c++11 -L/usr/local/lib -pthread -lgtest -lgtest_main

cl: $(SOURCE) Client_test.cpp
	clang++ $(SOURCE) Client_test.cpp  -o $(EXECFILE) -g -std=c++11 -L/usr/local/lib -pthread -lgtest -lgtest_main

gc: $(SOURCE) getChunked_test.cpp
	clang++ $(SOURCE) getChunked_test.cpp  -o $(EXECFILE) -g -std=c++11 -L/usr/local/lib -pthread -lgtest -lgtest_main

# re: $(SOURCE) Response_test.cpp
# 	g++ $(SOURCE) Response_test.cpp -o $(EXECFILE) -g -std=c++11 -L/usr/local/lib -pthread -lgtest -lgtest_main && rm /tmp/webserv/put/index.htm

re: $(OBJS) Response_test.cpp
	clang++ $(OBJS) Response_test.cpp -o test -g -std=c++11 -L/usr/local/lib -pthread -lgtest -lgtest_main && rm /tmp/webserv/put/index.htm && rm -rf /tmp/webserv/put/none/

clean:
	rm -rf *.o $(EXECFILE) test_ft