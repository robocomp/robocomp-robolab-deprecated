/*
* Javier Abellan, 20 Jun 2000
*
* Programa Cliente de un socket INET, como ejemplo de utilizacion
* de las funciones de sockets
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <tcp/Socket.h>
#include <tcp/Socket_Cliente.h>
#include <tcp/angulos.h>


main ()
{
	/*
	* Descriptor del socket y buffer para datos
	*/
	int Socket_Con_Servidor;
	char Cadena[100];

	int i, j;

	ANGULOS ang;

	/*
	* Se abre la conexion con el servidor, pasando el nombre del ordenador
	* y el servicio solicitado.
	* "localhost" corresponde al nombre del mismo ordenador en el que
	* estamos corriendo. Esta dado de alta en /etc/hosts
	* "cpp_java" es un servicio dado de alta en /etc/services
	*/
	Socket_Con_Servidor = Abre_Conexion_Inet ("localhost", "cpp_java");
	if (Socket_Con_Servidor == -1)
	{
		printf ("No puedo establecer conexion con el servidor\n");
		exit (-1);
	}

//	/*
//	* Se prepara una cadena con 5 caracteres y se envia, 4 letras mas
//	* el \0 que indica fin de cadena en C
//	*/
//	strcpy (Cadena, "Hola");
//	Escribe_Socket (Socket_Con_Servidor, Cadena, 5);
//
//	/*
//	* Se lee la informacion enviada por el servidor, que se supone es
//	* una cadena de 6 caracteres.
//	*/
//	Lee_Socket (Socket_Con_Servidor, Cadena, 6);
//
//	/*
//	* Se escribe en pantalla la informacion recibida del servidor
//	*/
//	printf ("Soy cliente, He recibido : %s\n", Cadena);

	for(i=0; i<5; i++){
		ang.control = INTERMEDIO;
		for(j=0;j<20;j++)
			ang.angulo[j] = i*j;

		EnviarAngulos(&ang, Socket_Con_Servidor);
		printf("Soy cliente; he enviado 20 ángulos\n");
	}

	ang.control = ULTIMO;
	ang.angulo[0] = 0.9878;
	ang.angulo[19] = 1.386;

	EnviarAngulos(&ang, Socket_Con_Servidor);
	printf("Soy cliente; he enviado 20 ángulos\n");

	/*
	* Se cierra el socket con el servidor
	*/
	close (Socket_Con_Servidor);
}
