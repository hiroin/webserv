CC = clang++
EXECFILE = test
SOURCE = ../../HTTPMessageParser.cpp \
	../../ft_istchar.cpp \
	../../ft_isspase_and_htab.cpp \
	../../ft_isvchar.cpp \
	../../ft_trim.cpp \
	../../ft_split_use_find_first_of.cpp \
	../../ft_isunreserved.cpp \
	../../ft_issubdelims.cpp \
	../../ft_stoi.cpp \
	../../ft_istoken.cpp \
	../../Client.cpp \
	../../recvData.cpp \
	../../ft_memset.cpp \
	../../ft_isctl.cpp \
	../../ft_istspecials.cpp \
	../../getChunked.cpp \
	../../Response.cpp \
  ../../Config.cpp \
	../../parseConfig.cpp \
	../../readData.cpp \
	../../php-cgi.cpp \
	../../normal-cgi.cpp \
	../../cgiTester.cpp
OBJS = $(SOURCE:.cpp=.o)

.PHONY: all clean

re: $(SOURCE) test.cpp
	$(CC) $(SOURCE) test.cpp  -o $(EXECFILE) -g -D_GLIBCXX_DEBUG -std=c++11 -fsanitize=address
# re: $(OBJS) test.cpp
# 	$(CC) $(OBJS) test.cpp  -o $(EXECFILE) -g -D_GLIBCXX_DEBUG -std=c++11 -fsanitize=address

clean:
	rm -rf *.o $(EXECFILE) test_ft