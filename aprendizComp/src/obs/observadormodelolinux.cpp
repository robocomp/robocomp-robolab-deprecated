#include "observadormodelo.h"

#include <tcp/Socket.h>
#include <tcp/Socket_Cliente.h>
#include <tcp/angulos.h>

#include <unistd.h>
#include <stdexcept>

#include <modelo-osg/modelo.h>
//#include <obs/conversiones.h>

#include <stdio.h>
#include <iostream>
using namespace std;

#define PI 3.1415926539
//#define UNA_DE_CADA_TANTAS 18


namespace osgDTE {

  observadormodelo::observadormodelo(int cada_tantas,
				     char* hostservidor,bool hayservidor)
    : contador(cada_tantas),
      contador_inicial(cada_tantas)
  {
    // Ojo esto puede lanzar excepcion...
    //angulos_brazoizquierdo.resize(4);
    //angulos_brazoderecho.resize(4);
  	

    // Abrir la conexion
    Socket_Con_Servidor = Abre_Conexion_Inet(hostservidor,"cpp_java");
    if (-1 == Socket_Con_Servidor) 
      {
	    //throw no_puedo_abrir_conexion();
            hayservidor=false;
      }
    else
      {
        hayservidor=true;
        printf("ABIERTA CONEXION\n");
      }

  }

  bool observadormodelo::updateFrom(Observado* obs)
  {
  	
    if (--contador > 1) return true;
    else contador = contador_inicial;
  	
    // De momento el observado es el proceso entero
    // Como luego se va a llamar al proceso ModeloPersonaAHoap, suponemos que
    // el observado es, en este caso, un modelo -básico, de los que se usan
    // para el tracking- de persona 
    modelo* _modelo = dynamic_cast<modelo*>(obs);
    if (_modelo)
      {
      	     	
	// TODO: Obtener angulos.
	//ModeloPersonaAHoap::AngulosBrazoIzquierdo(*_modelo, angulos_brazoizquierdo);
	//ModeloPersonaAHoap::AngulosBrazoDerecho(*_modelo, angulos_brazoderecho);

	// Mandarlos por el socket.
	ang.control = INTERMEDIO;
	
	ang.angulo[0]  =  0.0; //RLEG_JOINT[1]
	ang.angulo[1]  =  0.0; //RLEG_JOINT[2]
	ang.angulo[2]  =  0.0; //RLEG_JOINT[3]
	ang.angulo[3]  =  0.0; //RLEG_JOINT[4]
	ang.angulo[4]  =  0.0; //RLEG_JOINT[5]
	ang.angulo[5]  =  0.0; //RLEG_JOINT[6]
	ang.angulo[6]  =  _modelo->ang_right_arm[0]; //RARM_JOINT[1]
	ang.angulo[7]  =  _modelo->ang_right_arm[1]; //RARM_JOINT[2]
	ang.angulo[8]  =  _modelo->ang_right_arm[2]; //RARM_JOINT[3]
	ang.angulo[9]  =  _modelo->ang_right_arm[3]; //RARM_JOINT[4]
	ang.angulo[10] =  0.0; //LLEG_JOINT[1]
	ang.angulo[11] =  0.0; //LLEG_JOINT[2]
	ang.angulo[12] =  0.0; //LLEG_JOINT[3]
	ang.angulo[13] =  0.0; //LLEG_JOINT[4]
	ang.angulo[14] =  0.0; //LLEG_JOINT[5]
	ang.angulo[15] =  0.0; //LLEG_JOINT[6]
	ang.angulo[16] =  _modelo->ang_left_arm[0]; //LARM_JOINT[1]
	ang.angulo[17] =  _modelo->ang_left_arm[1]; //LARM_JOINT[2]
	ang.angulo[18] =  _modelo->ang_left_arm[2]; //LARM_JOINT[3]
	ang.angulo[19] =  _modelo->ang_left_arm[3]; //LARM_JOINT[4]
					
	// Mandarlos, ¡hombre ya!
	EnviarAngulos(&ang, Socket_Con_Servidor);
	//cout << "ENVIANDO ANGULOS " << endl;
		
      }
    else 
      {
	cerr << "Error en el observador: " << __FILE__ << __LINE__ << endl;
      }

    return true;
  }

  observadormodelo::~observadormodelo()
  {
  	
    ang.control = ULTIMO;
    EnviarAngulos(&ang, Socket_Con_Servidor);

    if (Socket_Con_Servidor != -1)
      {
	close (Socket_Con_Servidor);
      }
			

  	
  }

}
