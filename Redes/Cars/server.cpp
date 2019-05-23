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

struct player{
	int socket;
	int pos_fila;
	int pos_col;
	char avatar;
	int velocidad;
	player();
	player(int s, int c){
		socket = s;
		pos_col = c;
		pos_fila = 0;
		velocidad = 1;
	}
	player(int s, int c, char a){
		socket = s;
		pos_col = c;
		avatar = a;
		pos_fila = 0;
		velocidad = 1;
	}
	void agregar_avatar(char a){
		avatar = a;
	}
	void movimiento(char p){
		switch (p){
		case 'U':
			if(velocidad < 5) velocidad++;
			break;
		case 'D':
			if(velocidad > 1) velocidad--;
			break;
		case 'L':
			if(pos_col > 0) pos_col--;
			break;
		case 'R':
			if(pos_col < 4) pos_col++;
			break;
		}
	}
};

vector<player *> clientes;

void itoa(int n, char *c,int ini, int tam){
	for(int i=tam-1;i>=0;--i){
		c[i+ini]=n%10+'0';
		n=n/10;
	}
}

void mensaje_a_clientes(){
	for(;;){
		usleep(2000000);
		string carretera;
		carretera += 'R';
		string posiciones = "";
		for(int i = 0; i < clientes.size(); ++i){
			///cout << clientes[i]->socket << " " << clientes[i]->avatar << " " << clientes[i]->velocidad << " " << clientes[i]->pos_col << ", ";
			char *id = new char;
			char *pos_f = new char;
			char *pos_c = new char;
			itoa(clientes[i]->socket,id,0,2);
			itoa(clientes[i]->pos_fila,pos_f,0,2);
			itoa(clientes[i]->pos_col,pos_c,0,2);
			posiciones += (string(id) + string(pos_f) + string(pos_c));
		}
		char *t = new char;
		itoa(posiciones.size(), t, 0, 6);
		carretera+= string(t);
		carretera+= posiciones;
		cout << carretera << endl;
		cout << endl;
	}
}

bool avatar_en_uso(char a){
	for(int i = 0; i < clientes.size(); ++i){
		if(clientes[i]->avatar == a) return true;
	}
	return false;
}

void funcion(int SocketFD){
	char mensaje[255];
	clientes.push_back(new player(SocketFD,clientes.size()));
	int pos;
	for(int i = 0; i < clientes.size(); ++i){
		if(clientes[i]->socket == SocketFD) pos = i;
	}
	for(;;){
		read(SocketFD, mensaje, 255);
		char tipo_m = mensaje[0];
		char tam_m[6];
		for(int i = 0; i < 6; ++i){
			tam_m[i] = mensaje[i+1];
		}
		int ta = atoi(tam_m);
		char *men = new char;
		for(int i = 0; i < ta; ++i){
			*(men+i) = mensaje[i+7];
		}
		switch (tipo_m){
		case 'N':
			cout << "Avatar" << endl;
			if(avatar_en_uso(*men)){
				write(SocketFD, "Avatar en uso", 13);
				break;
			}
			clientes[pos]->agregar_avatar(*men);
			break;
		case 'M':
			cout << "Movimiento" << endl;
			clientes[pos]->movimiento(*men);
			break;
		}
		if(string(men) == "exit"){
			close(SocketFD);
			break;
		}
	}
	clientes.erase(clientes.begin()+pos);
}

void game(){
	while(clientes.size()!=4){}
	string mes = "G000006INICIO";
	for(int i = 0; i < clientes.size(); ++i){
		write(clientes[i]->socket, mes.c_str(), 13);
	}
	thread(mensaje_a_clientes).detach();
}

int main(){
	struct sockaddr_in stSockAddr;
	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
	
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(3222);
	stSockAddr.sin_addr.s_addr = INADDR_ANY;
	
	bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in));
	
	listen(SocketFD, 4);
	
	thread(game).detach();
	
	for(;;){
		int ConnectFD = accept(SocketFD, NULL, NULL);
		if(clientes.size() < 3){
			write(ConnectFD, "Ingrese su Avatar", 17);
			thread(funcion, ConnectFD).detach();
		}
		else{
			write(ConnectFD,"Sala Llena", 10);
			close(ConnectFD);
		}
	}
	
	close(SocketFD);
	return 0;
}
