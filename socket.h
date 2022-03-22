#include <eventloop.h>

class Socket
{
public:
    Socket(eventloop::Ptr);

    void connect(const std::string& url, int port);    
    void onconnected(int fd);

    int attachevent(int fd);
    int onread(int fd);
    int onwrite(int fd);
    int onerror(int fd);

private:
    int fd;

    eventloop::Ptr _poller;

    // onReadCb
    // onAccept
};