CC = clang++
HTTP1 = server
HTTP1_SOURCE = server.cpp socket.cpp http1header.cpp http1parser.cpp http1response.cpp ft_memset.cpp recvData.cpp HTTPMessageParser.cpp

.PHONY: all clean

all: $(HTTP1)

$(HTTP1): $(HTTP1_SOURCE)
	clang++ $(HTTP1_SOURCE) -o $(HTTP1) -g -std=c++98 -O0

clean:
	rm -rf *.o $(HTTP1)