#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

vector<int> clientes;

void itoa(int n, char *c,int ini, int tam){
    for(int i=tam-1;i>=0;--i){
        c[i+ini]=n%10+'0';
        n=n/10;
    }
}

void funcion(int SocketFD){
    cout << "Cliente conectado" << endl;
    clientes.push_back(SocketFD);
    struct sockaddr_in stSockAddr;
    int SocketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(10000 + clientes.size());
    stSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(SocketClient, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in));
    char mensaje[255];
    for(;;){
        read(SocketFD, mensaje, 255);
        char *num;
        for(int i = 0; i < 5; ++i){
            *(num+i) = mensaje[i+3];
        }
        int numero = atoi(num);
        numero+=7;
        char *m;
        itoa(numero, m, 0, 5);
        write(SocketClient, m, 5);
    }
}

int main(){
    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(3210);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in));

    listen(SocketFD, 4);

    for(;;){
        int ConnectFD = accept(SocketFD, NULL, NULL);
        thread(funcion, ConnectFD).detach();
    }

    close(SocketFD);
    return 0;
}
