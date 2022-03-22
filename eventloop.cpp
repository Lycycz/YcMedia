#include "eventloop.h"
#include <errno.h>
#include <stdio.h>

eventloop::eventloop(int *pair) : flag(true) {
  epollfd = epoll_create(5);
  int nRet = -1;

  // sockpair = pair;
  sockpair[0] = pair[0];
  sockpair[1] = pair[1];

  // nRet = socketpair(PF_UNIX, SOCK_STREAM, 0, pair);
}

void eventloop::addevent(int fd, int events, eventcb cb) {
  eventcontext *context = new eventcontext;
  context->fd = fd;
  context->events = events;
  context->cb = cb;

  epoll_event *event = new epoll_event;
  event->events = events;
  event->data.ptr = context;

  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, event);
}

void eventloop::delevent(int fd) {
  epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
}

void eventloop::loop() {

  // epoll_event event;
  // event.data.fd = sockpair[0];
  // event.events = EPOLLIN | EPOLLRDHUP;

  // epoll_ctl(epollfd, EPOLL_CTL_ADD, sockpair[0], &event);

  epoll_event events[5];

  while (flag) {
    int num = epoll_wait(epollfd, events, 5, 10);
    if (num < 0 && errno != EINTR) {
      printf("epoll failure\n");
      break;
    }

    for (int i = 0; i < num; i++) {
      eventcontext *context = (eventcontext *)events[i].data.ptr;
      int fd = context->fd;
      if (fd == sockpair[0]) {
        context->cb(fd, context->events);
      }
    }

    if (flag == false)
      printf("epoll end!\n");
    /* code */
  }
}

void eventloop::stop() { flag = false; }
