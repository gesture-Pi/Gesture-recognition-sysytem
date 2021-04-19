#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SERV_IP "127.0.0.1"
#define SERV_PORT 6666

using namespace std;
class socket_client
{
public:
	int readBuffer(uint8_t* buffer);
	int writeBuffer(string data);
	void exit();

};
