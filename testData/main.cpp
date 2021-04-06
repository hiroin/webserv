#include "Client.hpp"
#include "Config.hpp"
#include "HTTPMessageParser.hpp"

int main(int argc, char* argv[])
{
  Client client;
  Config c(1);
  HTTPMessageParser hmp(1);
  // HTTPMessageParser hmp("serverNames", 1);

}