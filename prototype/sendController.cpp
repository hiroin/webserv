#include "sendController.hpp"

void SendController::Initialize(int fd, char *buf, size_t len)
{
	CliantFd = fd;
	ResponseMessage = buf;
	LeftLength = len;
}

void SendController::setFd(int fd)
{
	CliantFd = fd;
}

void SendController::setSendData(char *buf, size_t len)
{
  ResponseMessage = buf;
  LeftLength = len;
}

bool SendController::SendMessage(size_t len)
{
	if (LeftLength <= len)
	{
    // std::cout << "ResponseMessage : " << ResponseMessage << std::endl;
		int r = write(CliantFd, ResponseMessage, LeftLength);
    if (r == -1)
      throw std::runtime_error("write error.");
    if (r != LeftLength)
    {
      ResponseMessage += r;
      LeftLength -= r;
      return false;
    }
		LeftLength = 0;
		return (true);
	}
	else
	{
		int r = write(CliantFd, ResponseMessage, len);
    if (r == -1)
      throw std::runtime_error("write error.");
		ResponseMessage += r;
		LeftLength -= r;
		return (false);
	}
}

int		SendController::GetFd()
{
	return (CliantFd);
}