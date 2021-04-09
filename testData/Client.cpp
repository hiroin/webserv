#include "Client.hpp"

Client::Client()
{
  HTTPMessageParser hmp(1);
  hmp_ = hmp;
}

Client::~Client()
{

}