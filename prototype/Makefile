CC = clang++
HTTP1 = server
HTTP1_SOURCE = server.cpp \
               Socket.cpp \
               recvData.cpp \
               HTTPMessageParser.cpp \
               Client.cpp \
               ft_memset.cpp \
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
							 ft_inet_ntoa.cpp
.PHONY: all clean

all: $(HTTP1)

$(HTTP1): $(HTTP1_SOURCE)
	clang++ $(HTTP1_SOURCE) -o $(HTTP1) -g -std=c++98 -O0

clean:
	rm -rf *.o $(HTTP1)