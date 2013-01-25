/*
Descripcion: Maneja la camara. Abre el fujo de video y captura frames del video.
Lenguaje de programacion utilizado: OpenCV
Autores: Alejandro Hidalgo Paniagua, Pablo Manzano Gómez
Fecha: 24 de Marzo de 2011.
*/

#include "camara.h"
#include <cv.h>
#include "highgui.h"

Camara::Camara()
{
  capture=cvCreateCameraCapture(0);
  
}

IplImage* Camara::getFrame()
{
  return cvQueryFrame(capture);
}

void Camara::IplImageToOsgImage(IplImage *imgOrigen, std::vector<uchar> &imgDestino)
{
  IplImage *imAux;
  imAux = cvCreateImage(cvGetSize(imgOrigen),IPL_DEPTH_8U,3);
  
  cvCvtColor(imgOrigen,imAux,CV_RGB2BGR);
  memcpy(&imgDestino[0],imAux->imageData,imgDestino.size()); 
  
  cvReleaseImage(&imAux);
}

Camara::~Camara()
{
  
}
