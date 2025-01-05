/*
struct sockaddr_in {
  short sin_family; // Familia de direcciones (AF_INET para IPv4)
  unsigned short sin_port; // Puerto (en formato de red, usamos htons para convertirlo)
  struct in_addr sin_addr; // Dirección IP (usamos INADDR_ANY o una dirección IP específica)
  char sin_zero[8]; // Relleno para hacer la estructura del tamaño correcto
};


*/

#include <iostream> //Para imprimir en consola
#include <cstring>  //Para manejar cadenas de caracteres
#include <sys/socket.h>     //Librería para crear sockets
#include <netinet/in.h>     //Para definir direcciones IP y puertos
#include <unistd.h> //Para funciones como close() 

#define PORT 8080       //Definimos el puerto que el servidor va a usar

using namespace std;

int main(){

    int server_fd, new_socket;
    struct sockaddr_in address;     //Estructura para manejar la dirección y el puerto
    int opt = 1;    //Opción usada para la configuración del socket
    int addrlen = sizeof(address);
    char buffer[1024] = {0};    //Un buffer para almacenar los datos recibidos del cliente

    //Crear socket de servidor
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        //Crea un nuevo socket, que es esencialmente un punto de comunicación en una red.
        //En este caso, se pasa un 0, lo que indica que se seleccionará el protocolo predeterminado para el tipo de socket especificado (TCP)
        cerr << "Error al crear socket" << endl;
        return -1;

    }

    //Configurar opciones del socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        //Se utiliza para configurar opciones en el socket. Permite modificar el comportamiento del socket a través de una serie de opciones.
        cerr << "Error en Opciones del Socket" << endl;
        return -1;

    }

    //Configurar la dirección y el puerto del servidor
    address.sin_family = AF_INET;   //Tipo de dirección: IPv4
    address.sin_addr.s_addr = INADDR_ANY;   //El servidor acepta conexiones de cualquier dirección IP

    address.sin_port = htons(PORT);     //Definir el puerto del servidor, en este caso 8080

    //Asociar (bind) el socket a la dirección y puerto definidos
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        //Se usa para asociar el socket a una dirección IP y un número de puerto.

        cerr << "Error en bind" << endl;
        return -1;

    }

    //Indicar que el servidor está listo para recibir conexiones
    if(listen(server_fd, 3) < 0){
        //3 -> backlog es el número máximo de conexiones pendientes que pueden estar en cola mientras el servidor está ocupado atendiendo otras conexiones. Si el número de solicitudes de conexión pendientes supera este valor, las conexiones adicionales pueden ser rechazadas o ignoradas hasta que se acepten algunas de las pendientes.
        cerr << "Error en Liste" << endl;
        return -1;

    }

    cout << "Esperando conexiones..." << endl;

    //Aceptar una conexión de un cliente
    if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){

        cerr << "Error en accept" << endl;
        return -1;

    }

    //Recibir un mensaje del cliente
    int valread = recv(new_socket, buffer, 1024, 0);

    if (valread > 0) {

        cout << "Respuesta recibida: " << buffer << endl;

    } else {

        cerr << "Error al recibir datos" << endl;
        return -1;
    }

    //Enviar una respuesta al cliente
    const char *message = "Hola desde el servidor :D";
    send(new_socket, message, strlen(message), 0);
    cout << "Mensaje enviado" << endl;

    //Cerrar la conexión con el cliente y el socket del servidor
    close(new_socket);
    close(server_fd);

    return 0;

}
