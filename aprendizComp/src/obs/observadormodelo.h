#ifndef OBSV_MODELO_H
#define OBSV_MODELO_H

#include <obs/Observador.h>
#include <tcp/angulos.h>
#include <vector>

namespace osgDTE {
  

  /// Error de no poder abrir conexion, el pobre.
  struct no_puedo_abrir_conexion{};


  /// Observa como se mueve el modelo y manda los angulos por un socket
  class observadormodelo : public Observador
    {
    public:

      /// Constructor intenta abrir conexión y si falla lanza error no_puedo_abrir_conexion;
      /// @param cada_tantas: Se mandan 1 de cada_tantas frames al servidor
      //observadormodelo(int cada_tantas,char* hostname = "dte012pta");
      observadormodelo(int cada_tantas,char* hostname,bool hayservidor);

      /// Función que llama el observado, o sea, el modelo, cada vez que este cambia.
      bool updateFrom(Observado* obs);

      /// Destructor cierra la conexión.
      virtual ~observadormodelo();

    private:
      
      /// Identificador del socket
      int Socket_Con_Servidor;

      /// Para guardar angulos
      std::vector<double> angulos_brazoizquierdo;
      std::vector<double> angulos_brazoderecho;
	  
      /// Angulos por el socket
      ANGULOS ang;

      /// Contador para mandar uno de cada tantos angulos
      int contador;
      int contador_inicial;
      
    };
}

#endif
