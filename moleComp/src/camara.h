/*
Descripcion: Maneja la camara. Abre el fujo de video y captura frames del video.
Lenguaje de programacion utilizado: OpenCV
Autores: Alejandro Hidalgo Paniagua, Pablo Manzano Gómez
Fecha: 24 de Marzo de 2011.
*/


#include <GL/glut.h>
#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <highgui.h>
#include <iostream>

#ifndef CAMARA_H
#define CAMARA_H

using namespace std;

class Camara
{
	private:
		
		//Se utiliza para obtener el flujo de de video de la camara
		CvCapture *capture;
		
	public:
	
		//Constructor de la clase
		Camara();
		
		//Obtiene un frame del flujo de video de la camara
		IplImage* getFrame();
		
		//Convierte una imagen ARUint8* a IplImage
		void IplImageToOsgImage(IplImage *imgOrigen, std::vector<uchar> &imgDestino);
		
		//Destructor de la clase
		~Camara();
};

#endif // CAMARA_H
