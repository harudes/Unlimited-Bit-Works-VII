#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <thread>
#include <vector>
#include <fstream>

using namespace std;

void itoa(int n, char *c,int ini, int tam){
	for(int i=tam-1;i>=0;--i){
		c[i+ini]=n%10+'0';
		n=n/10;
	}
}

void quiz(int id, int port){
	struct sockaddr_in stSockAddr, stSockAddr2;
	int SocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int SocketFD2 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	int number;

	char buffer1[255], buffer2[255];
	bzero(buffer1,255);
	bzero(buffer2,255);

	memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
	memset(&stSockAddr2, 0, sizeof(struct sockaddr_in));

	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(3210);
	stSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	stSockAddr2.sin_family = AF_INET;
	stSockAddr2.sin_port = htons(10000+port);
	stSockAddr2.sin_addr.s_addr = INADDR_ANY;

	bind(SocketFD2,(const struct sockaddr *)&stSockAddr2, sizeof(struct sockaddr_in));

	listen(SocketFD2, 4);

	connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in));

	int ConnectFD = accept(SocketFD2, NULL, NULL);

	ofstream outFile("file"+to_string(port));

	for(int i=0; i<20000;++i){
		itoa(id,buffer1,0,2);
		buffer1[2]=',';
		itoa(rand()%10000+1,buffer1,3,5);
		write(SocketFD,buffer1,8);
		bzero(buffer1,255);
		while(buffer2[0]==0){
			read(ConnectFD,buffer2,5);
		}
		number=atoi(buffer2);
		bzero(buffer2,255);
		outFile<<number<<endl;
	}

	shutdown(SocketFD, SHUT_RDWR);

	close(SocketFD);
	close(SocketFD2);
}

int main(){
	srand(time(NULL));
	thread(quiz,rand()%99+1,1).detach();
	thread(quiz,rand()%99+1,2).detach();
	thread(quiz,rand()%99+1,3).detach();
	return 0;
}
