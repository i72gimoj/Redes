#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <unistd.h>
#include <ctime>
#include <arpa/inet.h>
#include <fstream>
/*
#include "src/domino/domino.hpp"
#include "src/domino/tablero/tablero.hpp"
#include "src/domino/ficha/ficha.hpp"
*/

#define MSG_SIZE 250
#define MAX_CLIENTS 30

using namespace std;

struct Mesa
{
    int jugador1=-1;
    int jugador2=-1;
    bool partida=false;
}mesa[15];

struct Jugador{
    int sk;
    bool validado=false;
}jugador[MAX_CLIENTS];

int user_esp=0;

/*
 * El servidor ofrece el servicio de un chat para el domino, donde solo se meten comandos
 */
bool UsuarioConectado(char*,string);
bool existeUsuario(char *, string);
bool existeContrasena(char*,char*);
void manejador(int signum);
void salirCliente(int socket, fd_set *readfds, int *numClientes, int arrayClientes[], char usuario[30], char pass[30])/*, int socket_companero[], bool partida)*/;

int main()
{
    int sd, new_sd;
    struct sockaddr_in sockname, from;
    char buffer[250];
    socklen_t from_len;
    fd_set readfds, auxfds;
    int salida;
    int arrayClientes[MAX_CLIENTS];
    int numClientes = 0;
    //contadores
    int i, j;
    int recibidos;
    char identificador[MSG_SIZE];
    int usuario_espera;
    bool validado=false;
    
    int socket_companero[2];
    

    //eliminar para dejar el fichero con los usuarios y que solo necesites meter el usuario para entrar
    ofstream fichero("usuarios.txt", ios::trunc);
    fichero.close();
    ofstream fich("conectados.txt", ios::trunc);
    fich.close();

    struct Registro
    {
        char usuario[30] = "";
        char pass[30] = "";
    } registro;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1)
    {
        perror("No se puede abrir el socket servidor");
        exit(-1);
    }

    sockname.sin_family = AF_INET;
    sockname.sin_port = htons(2050);
    sockname.sin_addr.s_addr = INADDR_ANY;

    if (bind(sd, (struct sockaddr *)&sockname, sizeof(sockname)) == -1)
    {
        perror("Error en la operación bind");
        exit(1);
    }

    /*---------------------------------------------------------------------
		De las peticiones que vamos a aceptar sólo necesitamos el 
		tamaño de su estructura, el resto de información (familia, puerto, 
		ip), nos la proporcionará el método que recibe las peticiones.
   	----------------------------------------------------------------------*/
    from_len = sizeof(from);

    if (listen(sd, 1) == -1)
    {
        perror("Error en la operación de listen");
        exit(1);
    }

    //Inicializar los conjuntos fd_set
    FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd, &readfds);
    FD_SET(0, &readfds);
    //Capturamos la señal SIGINT (Ctrl+c)
    signal(SIGINT, manejador);

    /*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
    while (1)
    {

        //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)

        auxfds = readfds;

        salida = select(FD_SETSIZE, &auxfds, NULL, NULL, NULL);

        if (salida > 0)
        {

            for (i = 0; i < FD_SETSIZE; i++)
            {

                //Buscamos el socket por el que se ha establecido la comunicación
                if (FD_ISSET(i, &auxfds))
                {
                    if (i == sd)
                    {

                        if ((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1)
                        {
                            perror("Error aceptando peticiones");
                        }
                        else
                        { //Si hay espacio se inicia la conexion
                            if (numClientes < MAX_CLIENTS)
                            {
                                arrayClientes[numClientes] = new_sd;
                                numClientes++;
                                FD_SET(new_sd, &readfds);

                                strcpy(buffer, "+Ok, usuario conectado\n");

                                send(new_sd, buffer, strlen(buffer), 0);
                            }
                            else
                            {
                                bzero(buffer, sizeof(buffer));
                                strcpy(buffer, "Demasiados clientes conectados\n");
                                send(new_sd, buffer, strlen(buffer), 0);
                                close(new_sd);
                            }
                        }
                    }
                    else if (i == 0)
                    {
                        //Se ha introducido información de teclado
                        bzero(buffer, sizeof(buffer));
                        fgets(buffer, sizeof(buffer), stdin);

                        //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                        if (strcmp(buffer, "SALIR\n") == 0)
                        {

                            for (j = 0; j < numClientes; j++)
                            {
                                send(arrayClientes[j], "Desconexion servidor\n", strlen("Desconexion servidor\n"), 0);
                                close(arrayClientes[j]);
                                FD_CLR(arrayClientes[j], &readfds);
                            }
                            close(sd);
                            exit(-1);
                        }
                        //Mensajes que se quieran mandar a los clientes
                    }
                    else
                    { //Mensajes que escriben los clientes

                        bzero(buffer, sizeof(buffer));

                        recibidos = recv(i, buffer, sizeof(buffer), 0);

                        if (recibidos > 0)
                        {

                            if (strcmp(buffer, "SALIR\n") == 0)
                            {

                                salirCliente(i, &readfds, &numClientes, arrayClientes, registro.usuario, registro.pass)/*, socket_companero, mesa[i].partida)*/;
                            }
                            else
                            {
                                //Separa la cadena en palabras
                                char almacenar[20][20];
                                int num_palabras = 0;
                                int pos = 0;
                                for (int h = 0; h < (int)strlen(buffer); h++)
                                {
                                    if ((buffer[h] == ' ') || (buffer[h] == '|') || (buffer[h] == ',') || (buffer[h] == '\n'))
                                    {
                                        almacenar[num_palabras][pos] = '\0';
                                        num_palabras++;
                                        pos = 0;
                                        h++;
                                    }
                                    almacenar[num_palabras][pos] = buffer[h];
                                    pos++;
                                }
                                almacenar[num_palabras][pos] = '\0';

                                //////////////////////////////////////////////////
                                sprintf(identificador, "%d: %s", i, buffer);
                                bzero(buffer, sizeof(buffer));
                                strcpy(buffer, identificador);

                                //-------------------------------------------------

                                if ((strcmp(almacenar[0], "REGISTRO") == 0) && (strcmp(almacenar[1], "-u") == 0) && (strcmp(almacenar[3], "-p") == 0))
                                {
                                    ofstream fich("usuarios.txt", ios::app);
                                    if (!existeUsuario(almacenar[2],"usuarios.txt"))
                                    {
                                        strcpy(buffer, "+Ok. Usuario validado\n");
                                        strcpy(registro.usuario, almacenar[2]);
                                        strcpy(registro.pass, almacenar[4]);
                                        fich << almacenar[2] << " " << almacenar[4] << endl;
                                        send(i, buffer, strlen(buffer), 0);
                                    }
                                    else
                                    {
                                        strcpy(buffer, "-ERR. Error al registrar\n");
                                        send(i, buffer, strlen(buffer), 0);
                                    }
                                    fich.close();
                                }

                                else if (strcmp(almacenar[0], "USUARIO") == 0)
                                {
                                    bzero(registro.usuario, sizeof(registro.usuario));
                                    if (existeUsuario(almacenar[1],"usuarios.txt") && !UsuarioConectado(almacenar[1], "conectados.txt"))
                                    {
                                        strcpy(buffer, "+Ok. Usuario correcto\n");
                                        strcpy(registro.usuario, almacenar[1]);
                                        ofstream fich("conectados.txt", ios::app);
                                        fich << almacenar[1] << endl;
                                        fich.close();
                                        send(i, buffer, strlen(buffer), 0);
                                    }
                                    else
                                    {
                                        strcpy(buffer, "-ERR. Usuario incorrecto\n");
                                        send(i, buffer, strlen(buffer), 0);
                                    }
                                }
                                else if (strcmp(almacenar[0], "PASSWORD") == 0)
                                {
                                    if (strcmp(registro.usuario, "") == 0)
                                    {
                                        strcpy(buffer, "-ERR. Error en la validacion. Contrasena antes que usuario\n");
                                    }
                                    else
                                    {
                                        bzero(registro.pass,sizeof(registro.pass));
                                        strcpy(registro.pass,almacenar[1]);
                                        if(existeContrasena(registro.usuario,registro.pass)){
                                            strcpy(buffer, "+Ok. Usuario validado\n");
                                            validado = true;
                                        }
                                        else{
                                            strcpy(buffer,"-ERR. Error en la contrasena\n");
                                        }
                                    }
                                    send(i, buffer, strlen(buffer), 0);
                                }
                                else if ((strcmp(almacenar[0], "INICIAR-PARTIDA") == 0) && (validado == true))
                                {
                                    if (user_esp==1)
                                    {
                                        strcpy(buffer, "+Ok. Empieza la partida\n");
                                        mesa[i].partida=true;
                                        send(i, buffer, strlen(buffer), 0);
                                        user_esp--;
                                        //socket_companero[1]=new_sd;
                                        mesa[i].jugador1=usuario_espera;
                                        mesa[i].jugador2=new_sd;
                                        for(j=0; j<numClientes; j++)
                                            if(arrayClientes[j]==usuario_espera)
                                                send(arrayClientes[j], buffer, strlen(buffer), 0);
                                    }
                                    else
                                    {
                                        strcpy(buffer, "+Ok. Peticion recibida. Quedamos a la espera de mas jugadores\n");
                                        usuario_espera=new_sd;
                                        user_esp++;
                                       // socket_companero[0]=new_sd;
                                        send(i, buffer, strlen(buffer), 0);
                                    }
                                }

                               /*else if(((strcmp(almacenar[0],"COLOCAR-FICHA")==0) && (isdigit(atoi(almacenar[1]))) && (isdigit(atoi(almacenar[2]))) && ((strcmp(almacenar[2],"izquierda")) || (strcmp(almacenar[2],"derecha"))) && (partida==true))){
                                        
                                    }

                                    else if(strcmp(buffer,"ROBAR-FICHA")==0){
                                        if(){

                                        }
                                        else{
                                            strcpy(buffer,"+Ok. No es necesario robar ficha\n");
                                            send(new_sd,buffer,strlen(buffer),0);
                                        }
                                    }
                                    else if(strcmp(buffer,"PASO-TURNO")==0){
                                        if(){

                                        }
                                        else{
                                            strcpy(buffer,"+Ok. No es necesario pasar turno\n");
                                            send(new_sd,buffer,strlen(buffer),0);
                                        }
                                    }*/
                                ///-------------------------------------------------
                                else{
                                    strcpy(buffer, "-ERR. Comando invalido\n");
                                    send(new_sd, buffer, strlen(buffer), 0);
                                }
                            }
                        }
                        //Si el cliente introdujo ctrl+c
                        if (recibidos == 0)
                        {
                            cout << "El socket " << i << " ha introducido ctrl+c" << endl;
                            //Eliminar ese socket
                            salirCliente(i, &readfds, &numClientes, arrayClientes, registro.usuario, registro.pass)/*, socket_companero, mesa[i].partida)*/;
                        }
                    }
                }
            }
        }
    }

    close(sd);
    return 0;
}

bool UsuarioConectado(char *usuario, string fichero)
{
    ifstream fe(fichero);
    bool valor = false;
    char user[20];
    while (!fe.eof())
    {
        fe >> user;
        if (strcmp(user, usuario) == 0)
            valor = true;
    }
    fe.close();
    return valor;
}
bool existeUsuario(char *usuario, string fichero)
{
    ifstream fe(fichero);
    bool valor = false;
    char user[20], contrasena[20];
    while (!fe.eof())
    {
        fe >> user >> contrasena;
        if (strcmp(user, usuario) == 0)
            valor = true;
    }
    fe.close();
    return valor;
}
bool existeContrasena(char* usuario, char* contrasena){
    ifstream fe("usuarios.txt");
    bool valor=false;
    char user[20],pass[20];
    while(!fe.eof()){
        fe>>user>>pass;
        if ((strcmp(user,usuario)==0) && (strcmp(pass,contrasena)==0))
            valor=true;
        
    }
    fe.close();
    return valor;
}

void salirCliente(int socket, fd_set *readfds, int *numClientes, int arrayClientes[], char usuario[30], char pass[30])/*, int socket_companero[2], bool partida)*/
{

    char buffer[250];
    int j;

    close(socket);
    FD_CLR(socket, readfds);

    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j] == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j + 1]);

    (*numClientes)--;

   // if(partida==true){
        bzero(buffer, sizeof(buffer));
        strcpy(buffer, "+Ok. La partida ha sido anulada\n");
        for(int k=0; k<2; k++)
            for (j = 0; j < (*numClientes); j++)
                if (arrayClientes[j] != socket)
                    send(arrayClientes[j], buffer, strlen(buffer), 0);
    //}
}

void manejador(int signum)
{
    cout << "Se ha recibido la señal sigint" << endl;
    signal(SIGINT, manejador);

    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}