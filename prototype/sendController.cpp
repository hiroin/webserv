#include "sendController.hpp"

void SendController::Initialize(int fd, char *buf, ssize_t len)
{
	CliantFd = fd;
	ResponseMessage = buf;
	LeftLength = len;
}

void SendController::setFd(int fd)
{
	CliantFd = fd;
}

void SendController::setSendData(char *buf, ssize_t len)
{
  ResponseMessage = buf;
  LeftLength = len;
}

bool SendController::SendMessage(ssize_t len)
{
	if (LeftLength <= len)
	{
		ssize_t r = write(CliantFd, ResponseMessage, LeftLength);
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
		ssize_t r = write(CliantFd, ResponseMessage, len);
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