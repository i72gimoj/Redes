#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <string.h>


/*
 * El servidor ofrece el servicio de incrementar un n\ufffdmero recibido de un cliente
 */
 
int main ()
{
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int Socket_Servidor;
	struct sockaddr_in Servidor;
	char datos[50];
	time_t tiempo;
	struct tm * stTm;
	char cadena[100];
	
	tiempo = time(NULL);

	setlocale(LC_ALL,"");

	stTm = localtime(&tiempo);
	

	/* -----------------------------------------------------
   		Informaci\ufffdn del Cliente
   	-----------------------------------------------------*/
   	struct sockaddr_in Cliente;
	socklen_t Longitud_Cliente;
	
	
	/* --------------------------------------------------
		Se abre el socket Servidor
	---------------------------------------------------*/
	Socket_Servidor = socket (AF_INET, SOCK_DGRAM, 0);
	if (Socket_Servidor == -1)
	{
		printf ("No se puede abrir socket servidor\n");
		exit (-1);	
	}

	/* ------------------------------------------------------------------
		Se rellenan los campos de la estructura servidor, necesaria
		para la llamada a la funcion bind().
	-------------------------------------------------------------------*/
	Servidor.sin_family = AF_INET;
	Servidor.sin_port = htons(2000);
	Servidor.sin_addr.s_addr = htonl(INADDR_ANY); 

	if (bind (Socket_Servidor, (struct sockaddr *)&Servidor, sizeof (Servidor)) == -1)
	{
		close (Socket_Servidor);
		exit (-1);
	}
	

  	/*---------------------------------------------------------------------
		Del cliente s\ufffdlo necesitamos el tama\ufffdo de su estructura, el 
		resto de informaci\ufffdn (familia, puerto, ip), la obtendremos 
		nos la proporcionar\ufffd el propio m\ufffdtodo recvfrom, cuando
		recibamos la llamada de un cliente.
   ----------------------------------------------------------------------*/
	Longitud_Cliente = sizeof (Cliente);

   	
	/*-----------------------------------------------------------------------
		El servidor espera continuamente los mensajes de los clientes
	------------------------------------------------------------------------ */
	while (1)
	{
		printf("Leyendo...\n");
    /* -----------------------------------------------------------------
			Esperamos la llamada de alg\ufffdn cliente
		-------------------------------------------------------------------*/
      	int recibido = recvfrom (Socket_Servidor, (char*) &datos, sizeof(datos), 0,
			(struct sockaddr *) &Cliente, &Longitud_Cliente);



		/* -----------------------------------------------------------------
			Comprobamos si hemos recibido alguna informaci\ufffdn 
		-------------------------------------------------------------------*/
		if (recibido > 0)
		{


			if(strcmp(datos, "day")==0){
				strftime(cadena,100,"%A, %d de %B de %Y", stTm);
				printf ("Recibido %s, envio %s\n", datos, cadena);
			}
			if(strcmp(datos,"time")==0){
				strftime(cadena,100,"%H:%M:%S", stTm);
				printf ("Recibido %s, envio %s\n", datos, cadena);
			}
			if(strcmp(datos,"daytime")==0){
				strftime(cadena,100,"%A, %d de %B de %Y; %H:%M:%S", stTm);
				printf ("Recibido %s, envio %s\n", datos, cadena);
			}

    
			int enviado = sendto (Socket_Servidor, (char*) &cadena, sizeof(cadena), 0,
			(struct sockaddr *) &Cliente, Longitud_Cliente);

		}

    }
	 
	 close(Socket_Servidor);

	return 0;
}
