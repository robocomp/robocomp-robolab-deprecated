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
#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include <GL/glut.h>    
#include <AR/gsub.h>    
#include <AR/video.h>   
#include <AR/param.h>   
#include <AR/ar.h>
#include <AR/arMulti.h>
#include "gl.h"

// #include <qvision/qvision.h>

/**
       \brief
       @author authorname
*/

class SpecificWorker : public GenericWorker
{
Q_OBJECT
private:
	RoboCompCamera::TCamParams camParams;
	RoboCompCamera::imgType img;
	RoboCompCommonHead::THeadState hState;
	RoboCompDifferentialRobot::TBaseState bState;
	
	GLClass *gl;
//	QVision *visor;
	
	//ARToolkit
	ARUint8 *dataPtr;
	ARParam  wparam, cparam;
	ARMultiMarkerInfoT  *mMarker;     // Estructura global Multimarca
	ARMarkerInfo *marker_info;
	double patt_trans[3][4];   // Matriz de transformacion de la marca
	int marker_num, j, k;
	int    patt_id;            // Identificador unico de la marca
	double p_width;        // Ancho del patron (marca)
	double p_center[2];   // Centro del patron (marca)
	double error;               // Error en la deteccion multimarca
public:
	SpecificWorker(MapPrx& mprx);	
	~SpecificWorker();
	void setParams(RoboCompCommonBehavior::ParameterList params);
	void initAR();
	void cleanup();
	
public slots:
 	void compute(); 
};

#endif