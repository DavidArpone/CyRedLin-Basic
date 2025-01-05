#include<iostream>
#include<cstring>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 8080

using namespace std;

int main(int argc, char* argv[]){
	
	int sock = 0;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){

		cerr << "Error al crear el socket" << endl;
		return -1;

	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0){
		
		cerr << "Direccion invalida" << endl;
		return -1;

	}

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){

		cerr << "Error en la conexion" << endl;
		return -1;
	}

	const char *message = argv[2];

	send(sock, message, strlen(message), 0);

	cout << "Mensaje enviado" << endl;

	recv(sock, buffer, 1024, 0);

	cout << "Respuesta del servidor: " << buffer << endl;

	close(sock);
	return 0;


}
