#include "socket.h"

Socket::Socket(eventloop::Ptr poller) : _poller(poller) {}

void Socket::connect(const std::string &url, int port)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);

    _poller->addevent(fd, EPOLLOUT, [this](int fd, int events)
    {
        onconnected(fd);
    });
}

void Socket::onconnected(int fd)
{
    _poller->delevent(fd);
    attachevent(fd);
}

int Socket::attachevent(int fd)
{
    _poller->addevent(fd, EPOLLIN | EPOLLOUT | EPOLLERR, [this](int fd, int events)
    {
        if(events & EPOLLIN)
        {
            onread(fd);
        }
        if(events & EPOLLOUT)
        {
            onwrite(fd);
        }
        if(events & EPOLLERR)
        {
            onerror(fd);
        }

    });

    return 0;
}

int Socket::onread(int fd)
{
    char* buf = new char[1024];

    recv(fd, buf, 1024, 0);
    recvfrom()
}

int Socket::onwrite(int fd)
{
}

int Socket::onerror(int fd)
{
}