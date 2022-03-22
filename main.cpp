#include "eventloop.h"

#include <thread>
#include <unistd.h>
#include <netdb.h>

int main()
{
    struct addrinfo *answer=nullptr;
    std::string host = "0.0.0.0";
    int ret = getaddrinfo(host.c_str(), NULL, NULL, &answer);


    int* pair = new int[2];

    socketpair(PF_UNIX, SOCK_STREAM, 0, pair);

    eventloop loop(pair);

    auto cb = [](int fd, int events){
        char buf[1024];
        int num = recv(fd, buf, 1024, 0);
        if(num <= 0)
        {
            printf("recv failed!\n");
        }
        else
        {
            printf("socket read: %s!\n", buf);
        }
    };



    std::thread t1(&eventloop::loop, std::ref(loop));
    int fd = pair[0];
    loop.addevent(fd, EPOLLIN | EPOLLRDHUP, cb);


    sleep(5);

    const char* buf = "123";
    write(pair[1], buf, sizeof(buf));

    sleep(5);
    loop.stop();


    t1.join();
    return 0;
}