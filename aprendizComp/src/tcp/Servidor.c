/*
* Javier Abellan, 20 Jun 2000
*
* Programa Servidor de socket INET, como ejemplo de utilizacion de las
* funciones de sockets.
*/
#include <tcp/Socket_Servidor.h>
#include <tcp/Socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <tcp/angulos.h>

main ()
{
	/*
	* Descriptores de socket servidor y de socket con el cliente
	*/
	int Socket_Servidor;
	int Socket_Cliente;
	char Cadena[100];

	int i;

	ANGULOS ang;

	/*
	* Se abre el socket servidor, con el servicio "hoap_angulos" dado de
	* alta en /etc/services.
	*/
	Socket_Servidor = Abre_Socket_Inet ("cpp_java");
	if (Socket_Servidor == -1)
	{
		printf ("No se puede abrir socket servidor\n");
		exit (-1);
	}


while(1){
	/*
	* Se espera un cliente que quiera conectarse
	*/
	Socket_Cliente = Acepta_Conexion_Cliente (Socket_Servidor);
	if (Socket_Servidor == -1)
	{
		printf ("No se puede abrir socket de cliente\n");
		exit (-1);
	}

//	/*
//	* Se lee la informacion del cliente, suponiendo que va a enviar 
//	* 5 caracteres.
//	*/
//	Lee_Socket (Socket_Cliente, Cadena, 5);
//
//	/*
//	* Se escribe en pantalla la informacion que se ha recibido del
//	* cliente
//	*/
//	printf ("Soy Servidor, he recibido : %s\n", Cadena);
//
//	/*
//	* Se prepara una cadena de texto para enviar al cliente. La longitud
//	* de la cadena es 5 letras + \0 al final de la cadena = 6 caracteres
//	*/
//	strcpy (Cadena, "Adios");
//	Escribe_Socket (Socket_Cliente, Cadena, 6);


	ang.control=INTERMEDIO;
	while(ang.control==INTERMEDIO){
		RecibirAngulos(&ang, Socket_Cliente);
		printf("\n\nÁngulos recibidos\n");
		for(i=0; i<20; i++)
			printf("Ángulo %i=%f\n", i, ang.angulo[i]);
	}


	/*
	* Se cierran los sockets
	*/
	close (Socket_Cliente);
        if (ang.control == ULTIMO) break;//AÑADIDO IMPORTANTE
}

	close (Socket_Servidor);
}
