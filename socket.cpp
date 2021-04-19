#include "socket.h"

using namespace std;



int socket_client::readBuffer(uint8_t* buffer)
{
    int cfd;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZ];
    int n;

    cfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);

    connect(cfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    return read(cfd, buf, sizeof(buf));
        
}

int socket_client::writeBuffer(string data)
{
    int cfd;
    struct sockaddr_in serv_addr;

    cfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);

    connect(cfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    return send(cfd, data.c_str(), data.length(), 0);
}

void socket_client::exit()
{
    int cfd;
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    close(cfd);
}
