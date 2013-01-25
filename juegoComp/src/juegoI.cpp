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
#include "juegoI.h"

JuegoI::JuegoI(GenericWorker *_worker, QObject *parent) : QObject(parent)
{
	worker = _worker;
	mutex = worker->mutex;       // Shared worker mutex
	// Component initialization...
}


JuegoI::~JuegoI()
{
	// Free component resources here
}

// Component functions, implementation
void JuegoI::getARimg(imgType& img,int &width,int &height, const Ice::Current&){
	worker->getARimg(img, width,height);
}

void JuegoI::getJointsPosition(jointListType& jointList, const Ice::Current&){
	worker->getJointsPosition(jointList);
}

void JuegoI::getRTMatrixList(RTMatrixList& RTMatList, const Ice::Current&){
	worker->getRTMatrixList(RTMatList);
}

bool JuegoI::saveFile(const string& text, const string& header, const Ice::Current&){
	return worker->saveFile(text,header);
}

bool JuegoI::setJuego(const string& text, const Ice::Current&){
	return worker->setJuego(text);
}

void JuegoI::stopRecording(const Ice::Current&){
	worker->stopRecording();
}

void JuegoI::playVideo(const string& path, const Ice::Current&){
	worker->playVideo(path);
}

void JuegoI::makeVideoVisible(const Ice::Current&){
	worker->makeVideoVisible();
}

void JuegoI::makeARVisible(const Ice::Current&){
	worker->makeARVisible();
}

void JuegoI::stopVideo(const Ice::Current&){
	worker->stopVideo();
}

void JuegoI::pauseVideo(const Ice::Current&){
	worker->pauseVideo();
}

void JuegoI::playAr(const Ice::Current&){
	worker->playAr();
}

void JuegoI::setGameRange(const std::string& game,float min, float max,const Ice::Current& )
{
	worker->setGameRange(game,min,max);
}
void JuegoI::getGameRange(const std::string& game,float &min, float &max,const Ice::Current& )
{
	worker->getGameRange(game,min,max);
}
void JuegoI::setSceneObjectPosition(const std::string& object, int x, int y,int z,const Ice::Current&)
{
	worker->setSceneObjectPosition(object,x,y,z);
}
void JuegoI::getSceneObjectPosition(const std::string& object,int &x,int &y,int &z,const Ice::Current&)
{
	worker->getSceneObjectPosition(object,x,y,z);
}
