CXX		= clang++
HTTP1 = webserv
HTTP1_SOURCE = main.cpp \
               Socket.cpp \
               recvData.cpp \
               HTTPMessageParser.cpp \
               Client.cpp \
               ft_trim.cpp \
               ft_istchar.cpp \
               ft_isspase_and_htab.cpp \
               ft_isvchar.cpp \
               ft_split_use_find_first_of.cpp \
							 ft_print_vector_string.cpp \
							 ft_issubdelims.cpp \
							 ft_isunreserved.cpp \
							 Config.cpp \
							 parseConfig.cpp \
							 ft_istoken.cpp \
							 ft_isctl.cpp \
							 ft_istspecials.cpp \
							 ft_stoi.cpp \
							 ft_dummy_response.cpp \
							 getChunked.cpp \
							 sendController.cpp \
							 ft_make_dummy_response.cpp \
							 Response.cpp \
							 ft_inet_ntoa.cpp \
							 readData.cpp \
							 Webserv.cpp \
							 php-cgi.cpp \
							 normal-cgi.cpp \
							 cgiTester.cpp \
							 POST_normal-cgi.cpp \
							 POST_phpCGI.cpp \
							 POST_cgiTester.cpp
.PHONY: all clean
OBJS	= $(HTTP1_SOURCE:.cpp=.o)
.cpp.o:
	$(CXX) -Wall -Werror -Wextra -g -D_GLIBCXX_DEBUG -std=c++03 -c $< -o $@
all: $(HTTP1)
$(HTTP1): $(OBJS)
	clang++ $(OBJS) -o $(HTTP1) -g -D_GLIBCXX_DEBUG -std=c++03 -O0

clean:
	rm -rf *.o 
fclean: clean
	rm -rf $(HTTP1)
re: fclean all
