#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <fstream>

#define MSG_SIZE 250
#define MAX_CLIENTS 30

using namespace std;

/*
 * El servidor ofrece el servicio de un chat
 */

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);

int main(){
    int sd, new_sd;
	struct sockaddr_in sockname, from;
	char buffer[100], nuevo[30];
	socklen_t from_len;
    fd_set readfds, auxfds;
    int salida;
    int arrayClientes[MAX_CLIENTS];
    int numClientes = 0;
    //contadores
    int i,j,k;
	int recibidos;
    char identificador[MSG_SIZE];
    
    int on, ret;
	
	struct hostent * host;

    sd = socket (AF_INET, SOCK_STREAM, 0);
    if(sd == -1){
        perror("No se puede abrir el socket servidor");
        exit(-1);
    }
    //permite ejecutar varias veces seguidas el programa con el mismo puerto
    on=1;
    ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));


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
                                
                                    for(j=0; j<(numClientes-1);j++){
                                    
                                        bzero(buffer,sizeof(buffer));
                                        sprintf(buffer, "Nuevo Cliente conectado: %d\n",new_sd);
                                        send(arrayClientes[j],buffer,strlen(buffer),0);
                                    }
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
                            //Mensajes que se quieran mandar a los clientes (implementar)
                            
                            
                        } 
                        else{//Mensajes que escriben los clientes
                            bzero(buffer,sizeof(buffer));

                            string almacenar[50];
                            
                            recibidos = recv(i,almacenar,sizeof(almacenar),0);
                            
                            if(recibidos > 0){
                                
                                if(strcmp(buffer,"SALIR\n") == 0){
                                    
                                    salirCliente(i,&readfds,&numClientes,arrayClientes);
                                    
                                }
                                else{
                                    
                                    sprintf(identificador,"%d: %s",i,buffer);
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,identificador);
                                    
                                    for(j=0; j<numClientes; j++)
                                        if(arrayClientes[j] != i)
                                            send(arrayClientes[j],buffer,strlen(buffer),0);
                                    //-------------------------------------------------
                                    ifstream fe("usuarios.txt");
                                    if((strcmp(almacenar[0].c_str(),"PASSWORD") && strcmp(almacenar[1].c_str(), "-u")) && strcmp(almacenar[3].c_str(), "-p")){                                        
                                        strcpy(buffer, "+Ok. Usuario validado\n");
                                        send(new_sd,buffer,strlen(buffer),0);
                                        fe<<almacenar[2].c_str()<<"\t"<<almacenar[4].c_str()<<endl;
                                        bzero(almacenar.c_str(), sizeof(almacenar));
                                    }
                                    if(strcmp(almacenar[0].c_str(),"USUARIO")){
                                        strcpy(buffer,"+Ok. Usuario correcto\n");
                                        send(new_sd,buffer,strlen(buffer),0);
                                        if(strcmp(almacenar[0].c_str(),"PASSWORD")){                                        
                                            strcpy(buffer, "+Ok. Usuario validado\n");
                                            send(new_sd,buffer,strlen(buffer),0);
                                            fe<<almacenar[1].c_str()<<"\t"<<almacenar[3].c_str()<<endl; 
                                        }
                                        else{
                                            strcpy(buffer, "-ERR. Error en la validación\n");
                                            send(new_sd,buffer,strlen(buffer),0);
                                        }
                                    }
                                    fe.close();
                                    //-------------------------------------------------
                                    
                                }
                                                                
                                
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos== 0)
                            {
                                cout<<"El socket "<<i<<" ha introducido ctrl+c"<<endl;
                                //Eliminar ese socket
                                salirCliente(i,&readfds,&numClientes,arrayClientes);
                            }
                        }
                    }
                }
            }
		}

	close(sd);
	return 0;
	
}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]){
  
    char buffer[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j] == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j+1]);
    
    (*numClientes)--;
    
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente: %d\n",socket);
    
    for(j=0; j<(*numClientes); j++)
        if(arrayClientes[j] != socket)
            send(arrayClientes[j],buffer,strlen(buffer),0);


}


void manejador (int signum){
    cout<<"Se ha recibido la señal sigint"<<endl;
    signal(SIGINT,manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor

}