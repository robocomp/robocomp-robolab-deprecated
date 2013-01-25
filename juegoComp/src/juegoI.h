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
#ifndef JUEGOI_H
#define JUEGOI_H

// QT includes
#include <QtCore/QObject>

// Ice includes
#include <Ice/Ice.h>
#include <Juego.h>

#include <config.h>
#include "genericworker.h"

using namespace RoboCompJuego;

class JuegoI : public QObject , public virtual RoboCompJuego::Juego
{
Q_OBJECT
public:
	JuegoI( GenericWorker *_worker, QObject *parent = 0 );
	~JuegoI();
	void  getARimg(imgType& img,int &width,int &height, const Ice::Current& = Ice::Current());
	void  getJointsPosition(jointListType& jointList, const Ice::Current& = Ice::Current());
	void  getRTMatrixList(RTMatrixList& RTMatList, const Ice::Current& = Ice::Current());
	bool saveFile(const string& text, const string& header, const Ice::Current& = Ice::Current());
	bool setJuego(const string& text, const Ice::Current& = Ice::Current());
	void  stopRecording(const Ice::Current& = Ice::Current());
	void  playVideo(const string& path, const Ice::Current& = Ice::Current());
	void  makeVideoVisible(const Ice::Current& = Ice::Current());
	void  makeARVisible(const Ice::Current& = Ice::Current());
	void  stopVideo(const Ice::Current& = Ice::Current());
	void  pauseVideo(const Ice::Current& = Ice::Current());
	void  playAr(const Ice::Current& = Ice::Current());
	void setGameRange(const std::string& game,float min, float max,const Ice::Current& = Ice::Current());
	void getGameRange(const std::string& game,float &min, float &max,const Ice::Current& = Ice::Current());
	void setSceneObjectPosition(const std::string& object, int x, int y,int z,const Ice::Current& = Ice::Current());
	void getSceneObjectPosition(const std::string& object,int &x,int &y,int &z,const Ice::Current& = Ice::Current());
	QMutex *mutex;
private:

	GenericWorker *worker;
public slots:


};

#endif