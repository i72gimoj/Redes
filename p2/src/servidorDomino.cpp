#include <arpa/inet.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cctype>

#include "domino.hpp"

#define MSG_SIZE 250
#define MAX_CLIENTS 30

using namespace std;

struct Mesa
{
    int jugador1 = -1;
    int jugador2 = -1;
    bool partida = false;
    int turno;
    Domino partidaD;
};

struct Jugador
{
    int id;
    bool validado = false;
    int robadas = 0;
    Mesa *mesa;
};

int user_esp = 0;

/*
 * El servidor ofrece el servicio de un chat para el domino, donde solo se meten comandos
 */
bool UsuarioConectado(char *, string);
bool existeUsuario(char *, string);
bool existeContrasena(char *, char *);
void manejador(int signum);
void salirCliente(int socket, fd_set *readfds, vector<Jugador> &jugadores);

int main()
{
    int sd, new_sd;
    struct sockaddr_in sockname, from;
    char buffer[250];
    vector<Mesa> mesas;
    vector<Jugador> jugadores;
    Jugador jugador;
    Mesa mesa;
    socklen_t from_len;
    fd_set readfds, auxfds;
    int salida;
    int arrayClientes[MAX_CLIENTS];
    int numClientes = 0;
    // contadores
    int i, j;
    int recibidos;
    char identificador[MSG_SIZE];
    int usuario_espera;
    bool validado = false;

    int socket_companero[2];

    // eliminar para dejar el fichero con los usuarios y que solo necesites meter el usuario para entrar
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

    // Inicializar los conjuntos fd_set
    FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd, &readfds);
    FD_SET(0, &readfds);
    // Capturamos la señal SIGINT (Ctrl+c)
    signal(SIGINT, manejador);

    /*-----------------------------------------------------------------------
        El servidor acepta una petición
    ------------------------------------------------------------------------ */
    while (1)
    {

        // Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)

        auxfds = readfds;

        salida = select(FD_SETSIZE, &auxfds, NULL, NULL, NULL);

        if (salida > 0)
        {

            for (i = 0; i < FD_SETSIZE; i++)
            {

                // Buscamos el socket por el que se ha establecido la comunicación
                if (FD_ISSET(i, &auxfds))
                {
                    if (i == sd)
                    {

                        if ((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1)
                        {
                            perror("Error aceptando peticiones");
                        }
                        else
                        { // Si hay espacio se inicia la conexion
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
                        // Se ha introducido información de teclado
                        bzero(buffer, sizeof(buffer));
                        fgets(buffer, sizeof(buffer), stdin);

                        // Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del
                        // servidor. (implementar)
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
                        // Mensajes que se quieran mandar a los clientes
                    }
                    else
                    { // Mensajes que escriben los clientes

                        bzero(buffer, sizeof(buffer));

                        recibidos = recv(i, buffer, sizeof(buffer), 0);

                        if (recibidos > 0)
                        {

                            if (strcmp(buffer, "SALIR\n") == 0)
                            {

                                salirCliente(i, &readfds, jugadores);
                            }
                            else
                            {
                                // Separa la cadena en palabras
                                char almacenar[20][20];
                                int num_palabras = 0;
                                int pos = 0;
                                for (int h = 0; h < (int)strlen(buffer); h++)
                                {
                                    if ((buffer[h] == ' ') || (buffer[h] == '|') || (buffer[h] == ',') ||
                                        (buffer[h] == '\n'))
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

                                if ((strcmp(almacenar[0], "REGISTRO") == 0) && (strcmp(almacenar[1], "-u") == 0) &&
                                    (strcmp(almacenar[3], "-p") == 0))
                                {
                                    ofstream fich("usuarios.txt", ios::app);
                                    if (!existeUsuario(almacenar[2], "usuarios.txt"))
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
                                    if (existeUsuario(almacenar[1], "usuarios.txt") &&
                                        !UsuarioConectado(almacenar[1], "conectados.txt"))
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
                                        bzero(registro.pass, sizeof(registro.pass));
                                        strcpy(registro.pass, almacenar[1]);
                                        if (existeContrasena(registro.usuario, registro.pass))
                                        {
                                            strcpy(buffer, "+Ok. Usuario validado\n");
                                            validado = true;
                                            jugador.id = i;
                                            jugadores.push_back(jugador);
                                        }
                                        else
                                        {
                                            strcpy(buffer, "-ERR. Error en la contrasena\n");
                                        }
                                    }
                                    send(i, buffer, strlen(buffer), 0);
                                }
                                else if ((strcmp(almacenar[0], "INICIAR-PARTIDA") == 0) && (validado == true))
                                {
                                    if (user_esp == 1)
                                    {
                                        int turno;
                                        strcpy(buffer, "+Ok. Empieza la partida\n");
                                        send(i, buffer, strlen(buffer), 0);
                                        user_esp--;
                                        // socket_companero[1]=new_sd;
                                        mesa.jugador1 = usuario_espera;
                                        mesa.jugador2 = i;
                                        mesa.partida = true;
                                        mesa.partidaD = Domino(usuario_espera, i, turno);
                                        mesa.turno = turno;

                                        mesas.push_back(mesa);

                                        for (size_t i = 0; i < jugadores.size(); i++)
                                        {
                                            if (jugadores[i].id == i)
                                                jugadores[i].mesa = &mesas.back();
                                            if (jugadores[i].id == usuario_espera)
                                                jugadores[i].mesa = &mesas.back();
                                        }

                                        if (turno == usuario_espera)
                                        {
                                            strcpy(buffer, "+Ok. Empieza la partida, tu turno\n");
                                            send(usuario_espera, buffer, strlen(buffer), 0);
                                            strcpy(buffer, "+Ok. Empieza la partida, turno del rival\n");
                                            send(i, buffer, strlen(buffer), 0);
                                        }
                                        else
                                        {
                                            strcpy(buffer, "+Ok. Empieza la partida, tu turno\n");
                                            send(i, buffer, strlen(buffer), 0);
                                            strcpy(buffer, "+Ok. Empieza la partida, turno del rival\n");
                                            send(usuario_espera, buffer, strlen(buffer), 0);
                                        }
                                        for (size_t i = 0; i < mesas.size(); i++)
                                        {
                                            if (mesas[i].jugador1 == i or mesas[i].jugador2 == i)
                                                mesa = mesas[i];
                                        }

                                        send(i, mesa.partidaD.getTablero().c_str(),
                                             strlen(mesa.partidaD.getTablero().c_str()), 0);
                                        send(usuario_espera, mesa.partidaD.getTablero().c_str(),
                                             strlen(mesa.partidaD.getTablero().c_str()), 0);
                                        send(i, mesa.partidaD.getFichas(i).c_str(),
                                             strlen(mesa.partidaD.getFichas(i).c_str()), 0);
                                        send(usuario_espera, mesa.partidaD.getFichas(usuario_espera).c_str(),
                                             strlen(mesa.partidaD.getFichas(usuario_espera).c_str()), 0);
                                    }
                                    else
                                    {
                                        strcpy(buffer,
                                               "+Ok. Peticion recibida. Quedamos a la espera de mas jugadores\n");
                                        usuario_espera = i;
                                        user_esp++;
                                        // socket_companero[0]=new_sd;
                                        send(i, buffer, strlen(buffer), 0);
                                    }
                                }
                                else if ((strcmp(almacenar[0], "COLOCAR-FICHA") == 0)/* &&
                                          ((strcmp(almacenar[3], "izquierda") == 0) || (strcmp(almacenar[3], "derecha") == 0))*/)
                                {
                                    int cont = 0;
                                    bool salir = false;
                                    Ficha ficha(atoi(almacenar[1]), atoi(almacenar[2]));
                                    while (cont < jugadores.size() and salir == false)
                                    {
                                        if (jugadores[cont].id == i and i == jugadores[cont].mesa->turno)
                                        {

                                            send(i, mesa.partidaD.getTablero().c_str(),
                                                 strlen(mesa.partidaD.getTablero().c_str()), 0);
                                            send(i, mesa.partidaD.getFichas(i).c_str(),
                                                 strlen(mesa.partidaD.getFichas(i).c_str()), 0);

                                            strcpy(buffer, jugadores[cont]
                                                               .mesa->partidaD.ponerFicha(ficha, almacenar[3], i)
                                                               .c_str());
                                            salir = true;
                                            if (strcmp(buffer, "-Err. La ficha no puede ser colocada\n") == 0)
                                            {
                                                send(i, buffer, strlen(buffer), 0);
                                                salir = true;
                                            }
                                            else if (i != jugadores[cont].mesa->jugador1)
                                            {
                                                jugadores[cont].mesa->turno = jugadores[cont].mesa->jugador1;
                                                send(jugadores[cont].mesa->jugador1, buffer, strlen(buffer), 0);
                                                salir = true;
                                            }
                                            else
                                            {
                                                jugadores[cont].mesa->turno = jugadores[cont].mesa->jugador2;
                                                send(jugadores[cont].mesa->jugador2, buffer, strlen(buffer), 0);
                                                salir = true;
                                            }
                                        }
                                        cont++;
                                    }
                                }
                                else if (strcmp(buffer, "ROBAR-FICHA") == 0)
                                {
                                    Jugador jugador;
                                    int cont = 0;
                                    bool salir = false, robado = false;
                                    while (cont < jugadores.size() and salir == false)
                                    {
                                        if (jugadores[cont].id == i)
                                        {
                                            jugador = jugadores[cont];
                                            if (jugador.robadas == 1)
                                            {
                                                salir = true;
                                                send(i, "-Err. Ya ha robado una ficha.\n",
                                                     strlen("-Err. Ya ha robado una ficha.\n"), 0);
                                                robado = true;
                                            }
                                            else
                                                salir = true;
                                        }
                                    }

                                    cont = 0;
                                    while (cont < jugadores.size() and robado == false)
                                    {
                                        if (jugadores[cont].id == i and i == jugadores[cont].mesa->turno)
                                        {
                                            strcpy(buffer, jugadores[cont].mesa->partidaD.robarFicha(i).c_str());
                                            salir = true;
                                            if (strcmp(buffer, "+Ok. No es necesario robar ficha.\n") == 0)
                                            {
                                                send(i, buffer, strlen(buffer), 0);
                                                robado = true;
                                            }
                                            else if (i != jugadores[cont].mesa->jugador1)
                                            {
                                                send(jugadores[cont].mesa->jugador1, buffer, strlen(buffer), 0);
                                                robado = true;
                                            }

                                            else
                                            {
                                                send(jugadores[cont].mesa->jugador2, buffer, strlen(buffer), 0);
                                                robado = true;
                                            }
                                        }
                                        cont++;
                                    }
                                }
                                else if (strcmp(buffer, "PASO-TURNO") == 0)
                                {
                                    int cont = 0;
                                    bool salir = false;
                                    while (cont < jugadores.size() and salir == false)
                                    {
                                        if (jugadores[cont].id == i and i == jugadores[cont].mesa->turno)
                                        {
                                            if (jugadores[cont].mesa->jugador1 == i)
                                            {
                                                jugadores[cont].mesa->turno = jugadores[cont].mesa->jugador2;
                                                salir = true;
                                            }
                                            else
                                            {
                                                jugadores[cont].mesa->turno = jugadores[cont].mesa->jugador1;
                                                salir = true;
                                            }
                                        }
                                    }
                                }
                                ///-------------------------------------------------
                                else
                                {
                                    strcpy(buffer, "-ERR. Comando invalido\n");
                                    send(i, buffer, strlen(buffer), 0);
                                }
                            }
                        }
                        // Si el cliente introdujo ctrl+c
                        if (recibidos == 0)
                        {
                            cout << "El socket " << i << " ha introducido ctrl+c" << endl;
                            // Eliminar ese socket
                            salirCliente(i, &readfds, jugadores) /*, socket_companero, mesa[i].partida)*/;
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
bool existeContrasena(char *usuario, char *contrasena)
{
    ifstream fe("usuarios.txt");
    bool valor = false;
    char user[20], pass[20];
    while (!fe.eof())
    {
        fe >> user >> pass;
        if ((strcmp(user, usuario) == 0) && (strcmp(pass, contrasena) == 0))
            valor = true;
    }
    fe.close();
    return valor;
}

void salirCliente(int socket, fd_set *readfds, vector<Jugador> &jugadores) 
{

    char buffer[250];
    int cont = 0;
    bool salir = false;

    bzero(buffer, sizeof(buffer));
    strcpy(buffer, "+Ok. La partida ha sido anulada\n");

    close(socket);
    FD_CLR(socket, readfds);

    // Re-estructurar el array de clientes
    while (cont < jugadores.size() and !salir)
    {
        if (jugadores[cont].id == socket)
        {
            if (jugadores[cont].mesa->jugador1 == socket)
                send(jugadores[cont].mesa->jugador2, buffer, strlen(buffer), 0);
            else
                send(jugadores[cont].mesa->jugador1, buffer, strlen(buffer), 0);
            jugadores.erase(jugadores.begin() + cont);
        }

        cont++;
    }
}

void manejador(int signum)
{
    cout << "Se ha recibido la señal sigint" << endl;
    signal(SIGINT, manejador);

    // Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}