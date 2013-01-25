/*
 *    Copyright (C) 2006-2010 by RoboLab - University of Extremadura
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
 
 #include "specificworker.h"


/**
* \brief Default constructor
*/

SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)	
{
	try
	{
		camParams = camera->getCamParams();
	}
	catch(Ice::Exception e)
	{
		qDebug()<<"Error reading camera params: "<<e.what();
	}
	
	dataPtr = (uchar*)malloc(camParams.width*camParams.height*4);
	initAR();

//	visor =  new QVision(camParams.width,camParams.height,dataPtr,640,480 ,frame);
	timer.start(Period);
	 
	
	
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{

}
// static void loop(void  )
// {
// 	uchar *dataPtr = (uchar*)malloc(640*480*4);
// 	
// 	QImage image("/home/ramon/Escritorio/muneco-trapo.jpg");
// 	image = image.scaled(640,480);
// 	memcpy(dataPtr,image.bits(),640*480*3);
// 	
// 	argDrawMode2D();
// 	argDispImage(dataPtr, 0,0);    // Dibujamos lo que ve la camara
// 
// 
// }
void SpecificWorker::compute( )
{
	try
	{
		camera->getRGBPackedImage(0,img,hState,bState);
		memcpy(dataPtr,&img[0],camParams.width*camParams.height*3);
	}
	catch(Ice::Exception e)
	{
		qDebug()<<"Error reading camera params: "<<e.what();
	}
	QImage image(dataPtr,640,480,QImage::Format_RGB888);
	image.save("hola.png");
	
// 	argDrawMode2D();
// 	argDispImage(dataPtr, 0,0);    // Dibujamos lo que ve la camara

//	visor->update();

	


	qDebug()<<"compute";

// 	// Detectamos la marca en el frame capturado (return -1 si error)
// 	if(arDetectMarker(dataPtr, 120, &marker_info, &marker_num) < 0) {
// 		cleanup(); exit(0);   // Si devolvio -1, salimos del programa!
// 	}
// /*	if(arMultiGetTransMat(marker_info, marker_num, mMarker) > 0) 
// 		draw();       // Dibujamos los objetos de la escena*/
// 	
// 
// 		// Vemos donde detecta el patron con mayor fiabilidad
// 	for(j = 0, k = -1; j < marker_num; j++) {
// 		if(patt_id == marker_info[j].id) {
// 		if (k == -1) k = j;
// 		else if(marker_info[k].cf < marker_info[j].cf) k = j;
// 		}
// 	}
// 
// 	if(k != -1) {   // Si ha detectado el patron en algun sitio...
// 		// Obtener transformacion relativa entre la marca y la camara real
// 		arGetTransMat(&marker_info[k], p_center, p_width, patt_trans);
// //		drew();       // Dibujamos los objetos de la escena
// 	}
	
//	argSwapBuffers(); // Cambiamos el buffer con lo que tenga dibujado*/
}
void SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	timer.start(Period);
};


void SpecificWorker::initAR()
{	
	if(arParamLoad("data/camera_para.dat", 1, &wparam) < 0)   
		rError("Error en carga de parametros de camara\n");
	
	arParamChangeSize(&wparam, camParams.width, camParams.height, &cparam);
	arInitCparam(&cparam);   // Inicializamos la camara con "cparam"

	// Cargamos el fichero de especificacion multimarca
	if( (mMarker = arMultiReadConfigFile("data/marker.dat")) == NULL )
		rError("Error en fichero marker.dat (base)\n");

	if((patt_id=arLoadPatt("data/simple.patt")) < 0) 
		rError("Error en carga de patron\n");

	p_width     = 90.f;        // Ancho del patron (marca)
	p_center[0] = 0.f;
	p_center[1] = 0.f;   // Centro del patron (marca)
	
	gl = new GLClass(camParams.width,camParams.height,dataPtr,frame);
	gl->show();
}

void SpecificWorker::cleanup(){
	// Libera recursos al salir ...
	argCleanup();
	exit(0);
}
