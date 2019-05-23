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

vector<vector<char> > positions(15); 
vector<vector<char> > road(45); 

int printed=0;

void printMap(){
	++printed;
	bool aux=printed%2;
	for(size_t i=0;i<positions.size();++i)
		positions[i].assign(5,' ');
	positions[printed%15][0]='#';
	for(size_t i=0;i<road.size();++i)
		road[i].assign(18,' ');
	for(int i=0; i<positions.size(); ++i){
		for(int j=0; j<positions[i].size(); ++j){
			if(positions[i][j]!=' '){
				road[(i*3)][(j*3)+2]=positions[i][j];
				road[(i*3)][(j*3)+3]=positions[i][j];
				road[(i*3)+1][(j*3)+2]=positions[i][j];
				road[(i*3)+1][(j*3)+3]=positions[i][j];
				road[(i*3)+2][(j*3)+2]=positions[i][j];
				road[(i*3)+2][(j*3)+3]=positions[i][j];
			}
		}
	}
	for(int i=0;i<road.size();++i){
		if((i+0)%2==aux){
			road[i][0] ='|';
		}
		else{
			road[i][17 ]='|';
		}
	}
	for(size_t i=0;i<road.size();++i){
		for(size_t j=0;j<road[i].size();++j){
		  cout<<road[44-i][j];
		}
	cout<<endl;
	}
	system("clear");
}

void recepcion_del_servidor(int serv){
	char mensaje[255];
	for(;;){
		read(serv, mensaje, 255);
		cout << string(mensaje) << endl;
		if(string(mensaje) == "Sala Llena"){
			break;
		}
	}
	close(serv);
}

int main(){
	for(size_t i=0;i<positions.size();++i)
		positions[i].assign(5,' ');
	for(size_t i=0;i<road.size();++i)
		road[i].assign(18,' ');
	
	struct sockaddr_in stSockAddr;
	int SocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
	
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(3222);
	stSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in));
	thread(recepcion_del_servidor, SocketFD).detach();
	for(;;){
		string mensaje;
		getline(cin,mensaje);
		write(SocketFD, mensaje.c_str(), mensaje.size());
		if(mensaje == "E000004exit"){
			break;
		}
	}
	
	shutdown(SocketFD, SHUT_RDWR);
	
	close(SocketFD);
	return 0;
}
