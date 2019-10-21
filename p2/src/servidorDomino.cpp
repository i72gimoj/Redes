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

/*
 * El servidor ofrece el servicio de un chat para el domino, donde solo se meten comandos
 */

bool existeUsuario(char*);
void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[], char usuario[30], char pass[30]);

int main(){
    int sd, new_sd;
	struct sockaddr_in sockname, from;
	char buffer[250];
    bool validado=false;
	socklen_t from_len;
    fd_set readfds, auxfds;
    int salida;
    int arrayClientes[MAX_CLIENTS];
    int numClientes = 0;
    //contadores
    int i,j;
	int recibidos;
    char identificador[MSG_SIZE];

    ofstream fichero("usuarios.txt", ios::trunc);
    fichero.close();

    struct Registro{
        char usuario[30]="";
        char pass[30]="";
    }registro;
    
	

    sd = socket (AF_INET, SOCK_STREAM, 0);
    if(sd == -1){
        perror("No se puede abrir el socket servidor");
        exit(-1);
    }


	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  INADDR_ANY;

	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1)
	{
		perror("Error en la operación bind");
		exit(1);
	}

    /*---------------------------------------------------------------------
		De las peticiones que vamos a aceptar sólo necesitamos el 
		tamaño de su estructura, el resto de información (familia, puerto, 
		ip), nos la proporcionará el método que recibe las peticiones.
   	----------------------------------------------------------------------*/
	from_len = sizeof (from);

	if(listen(sd,1) == -1){
		perror("Error en la operación de listen");
		exit(1);
	}
    
    //Inicializar los conjuntos fd_set
    FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd,&readfds);
    FD_SET(0,&readfds);
    //Capturamos la señal SIGINT (Ctrl+c)
    signal(SIGINT,manejador);
    
	/*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
		while(1){
            
            //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
            
            auxfds = readfds;
            
            salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
            if(salida > 0){
                
                
                for(i=0; i<FD_SETSIZE; i++){
                    
                    //Buscamos el socket por el que se ha establecido la comunicación
                    if(FD_ISSET(i, &auxfds)) {
                        if( i == sd){
                            
                            if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                                perror("Error aceptando peticiones");
                            }
                            else
                            {   //Si hay espacio si inicia la conexion
                                if(numClientes < MAX_CLIENTS){
                                    arrayClientes[numClientes] = new_sd;
                                    numClientes++;
                                    FD_SET(new_sd,&readfds);
                                
                                    strcpy(buffer, "+Ok, usuario conectado\n");
                                
                                    send(new_sd,buffer,strlen(buffer),0);
                                
                                }
                                else
                                {
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"Demasiados clientes conectados\n");
                                    send(new_sd,buffer,strlen(buffer),0);
                                    close(new_sd);
                                }
                                
                            }
                            
                            
                        }
                        else if (i == 0){
                            //Se ha introducido información de teclado
                            bzero(buffer, sizeof(buffer));
                            fgets(buffer, sizeof(buffer),stdin);
                            
                            //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                            if(strcmp(buffer,"SALIR\n") == 0){
                             
                                for (j = 0; j < numClientes; j++){
                                    send(arrayClientes[j], "Desconexion servidor\n", strlen("Desconexion servidor\n"),0);
                                    close(arrayClientes[j]);
                                    FD_CLR(arrayClientes[j],&readfds);
                                }
                                close(sd);
                                exit(-1); 
                            }
                            //Mensajes que se quieran mandar a los clientes
                            
                            
                        } 
                        else{//Mensajes que escriben los clientes

                            bzero(buffer,sizeof(buffer));                            
                            
                            recibidos = recv(i,buffer,sizeof(buffer),0);
                            
                            if(recibidos > 0){
                                
                                if(strcmp(buffer,"SALIR\n") == 0){
                                    
                                    salirCliente(i,&readfds,&numClientes,arrayClientes,registro.usuario,registro.pass);
                                    
                                }
                                else{
                                    ///////////////////////////////////////////////
                                    char almacenar[20][20];
                                    int num_palabras=0;
                                    int pos=0;
                                    for (int h = 0; h < (int)strlen(buffer); h++){
                                        if((buffer[h] == ' ')||(buffer[h] == '|')||(buffer[h]==',') || (buffer[h]=='\n')){
                                            almacenar[num_palabras][pos]='\0'; 
                                            num_palabras++;
                                            pos=0;          
                                            h++;
                                        }
                                        almacenar[num_palabras][pos]=buffer[h];
                                        pos++;
                                    }
                                    almacenar[num_palabras][pos]='\0';
                                
                                    //////////////////////////////////////////////////
                                    sprintf(identificador,"%d: %s",i,buffer);
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,identificador);
                                    
                                    //-------------------------------------------------
                                    
                                    if((strcmp(almacenar[0],"REGISTRO")==0) && (strcmp(almacenar[1], "-u")==0) && (strcmp(almacenar[3], "-p")==0)){
                                        ofstream fich("usuarios.txt", ios::app);
                                        if(!existeUsuario(almacenar[2])){
                                            strcpy(buffer, "+Ok. Usuario validado\n");
                                            strcpy(registro.usuario,almacenar[2]);
                                            strcpy(registro.pass,almacenar[4]);
                                            fich<<almacenar[2]<<"\t"<<almacenar[4]<<endl;
                                            validado=true;
                                            send(new_sd,buffer,strlen(buffer),0);
                                        }
                                        else{
                                            strcpy(buffer,"-ERR. Error en la validacion\n");
                                            send(new_sd,buffer,strlen(buffer),0);             
                                        }
                                        fich.close();
                                    }
                                    
                                    if(strcmp(almacenar[0],"USUARIO")==0){
                                        bzero(registro.usuario,sizeof(registro.usuario));
                                        if(!existeUsuario(almacenar[1])){
                                            strcpy(buffer,"+Ok. Usuario correcto\n");
                                            strcpy(registro.usuario,almacenar[1]);
                                            send(new_sd,buffer,strlen(buffer),0);
                                        }
                                        else{
                                            strcpy(buffer,"-ERR. Usuario incorrecto\n");
                                            send(new_sd,buffer,strlen(buffer),0);
                                        }
                                    }
                                    if(strcmp(almacenar[0],"PASSWORD")==0){                                        
                                        if(strcmp(registro.usuario,"")==0){
                                            strcpy(buffer,"-ERR. Error en la validacion. Contrasena antes que usuario\n");
                                        }
                                        else{
                                            ofstream fichero("usuarios.txt", ios::app);
                                            strcpy(buffer,"+Ok. Usuario validado\n");
                                            validado=true;
                                            fichero<<registro.usuario<<"\t"<<almacenar[1]<<endl;
                                            fichero.close();
                                            strcpy(registro.pass,almacenar[1]);
                                        }
                                        send(new_sd,buffer,strlen(buffer),0);
                                    }
                                    if((strcmp(almacenar[0], "INICIAR-PARTIDA")==0) && (validado==true)){
                                        if(numClientes%2==0){
                                        strcpy(buffer, "+Ok. Empieza la partida\n");
                                            send(new_sd,buffer,strlen(buffer),0);
                                                                                       
                                            send(arrayClientes[numClientes-2],buffer,strlen(buffer),0);
                                        }
                                        else{
                                            strcpy(buffer, "+Ok. Peticion recibida. Quedamos a la espera de mas jugadores\n");
                                            send(new_sd,buffer,strlen(buffer),0); 
                                        }
                                    }
                                    
                                   /* if((strcmp(almacenar[0],"COLOCAR-FICHA")==0) && (isdigit(atoi(almacenar[1]))) && (isdigit(atoi(almacenar[2]))) && ((strcmp(almacenar[2],"izquierda")) || (strcmp(almacenar[2],"derecha")))){
                                        
                                    }

                                    if(strcmp(buffer,"ROBAR-FICHA")==0){
                                        if(){

                                        }
                                        else{
                                            strcpy(buffer,"+Ok. No es necesario robar ficha\n");
                                            send(new_sd,buffer,strlen(buffer),0);
                                        }
                                    }
                                    if(strcmp(buffer,"PASO-TURNO")==0){
                                        if(){

                                        }
                                        else{
                                            strcpy(buffer,"+Ok. No es necesario pasar turno\n");
                                            send(new_sd,buffer,strlen(buffer),0);
                                        }
                                    }*/
                                    ///-------------------------------------------------
                                    
                                }
                                                                
                         
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos== 0)
                            {
                                cout<<"El socket "<<i<<" ha introducido ctrl+c"<<endl;
                                //Eliminar ese socket
                                salirCliente(i,&readfds,&numClientes,arrayClientes,registro.usuario,registro.pass);
                            }
                        }
                    }
                }
            }
        }

	close(sd);
    return 0;
}

bool existeUsuario(char *usuario){
    ifstream fe("usuarios.txt");
    bool valor=false;
    char user[20], contrasena[20];
    while(!fe.eof()){
        fe>>user>>contrasena;
        if(strcmp(user,usuario)==0)
            valor=true;
    }
    fe.close();
    return valor;
}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[], char usuario[30], char pass[30]){
  
    char buffer[250];
    int j;
    char user[20], password[20];
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j] == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j+1]);
    
    (*numClientes)--;
    ifstream fichero("usuarios.txt");
    ofstream temp("temp.txt",ios::app);
    while(!fichero.eof()){
        fichero>>user>>password;
        if(strcmp(user,usuario)!=0){
            temp<<user<<"\t"<<password;
        }
        fichero>>user>>password;
    }
    temp.close();
    fichero.close();
    remove("usuarios.txt");
    rename("temp.txt","usuarios.txt");

    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"+Ok. La partida ha sido anulada\n");
    
    for(j=0; j<(*numClientes); j++)
        if(arrayClientes[j] != socket)
            send(arrayClientes[j],buffer,strlen(buffer),0);
}


void manejador (int signum){
    cout<<"Se ha recibido la señal sigint"<<endl;
    signal(SIGINT,manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor

}