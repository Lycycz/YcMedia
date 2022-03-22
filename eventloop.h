#include <functional>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <thread>

typedef std::function<void(int fd, int events)> eventcb;

typedef struct eventcontext
{
  int fd;
  int events;
  void *args;
  eventcb cb;
} eventcontext;

class eventloop
{
public:
  using Ptr = std::shared_ptr<eventloop>;

  eventloop(int *pair);

  void addevent(int fd, int events, eventcb cb);
  void delevent(int fd);

  void loop();
  void stop();

private:
  int epollfd;

  int sockpair[2];
  int flag;
};
