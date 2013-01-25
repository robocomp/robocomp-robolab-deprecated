#include <tcp/angulos.h>
#include <tcp/Socket.h>


int EnviarAngulos(ANGULOS *ang, int Socket){

	Escribe_Socket (Socket, (char *) ang, sizeof (ANGULOS));

}



int RecibirAngulos(ANGULOS *ang, int Socket){

	Lee_Socket (Socket, (char *) ang, sizeof (ANGULOS));

}
