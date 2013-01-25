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
#ifndef GENERICWORKER_H
#define GENERICWORKER_H

// #include <ipp.h>
#include <QtGui>
#include <stdint.h>
#include <qlog/qlog.h>
#include <CommonBehavior.h>
#include <ui_guiDlg.h>
#include <EvaluadorOff.h>
#include <UrsusArm.h>
#include <Speechgenerator.h>
#include <Speech.h>
#include <Trackeropenni.h>
#include <Juego.h>

#define CHECK_PERIOD 5000
#define BASIC_PERIOD 100

typedef map <string,::IceProxy::Ice::Object*> MapPrx;

using namespace std;

/**
       \brief
       @author authorname
*/
using namespace RoboCompJuego;

class GenericWorker : public QWidget, public Ui_guiDlg
{
Q_OBJECT
public:
	GenericWorker(MapPrx& mprx);
	virtual ~GenericWorker();
	virtual void killYourSelf();
	virtual void setPeriod(int p);
	
	virtual void setParams(RoboCompCommonBehavior::ParameterList params) = 0;
	QMutex *mutex;                //Shared mutex with servant

	RoboCompTrackeropenni::TrackeropenniPrx trackeropenni;
RoboCompEvaluadorOff::EvaluadorOffPrx evaluadoroff;
RoboCompSpeech::SpeechPrx speech;
RoboCompUrsusArm::UrsusArmPrx ursusarm;
RoboCompSpeechgenerator::SpeechgeneratorPrx speechgenerator;
virtual void  getARimg(imgType& img,int &width,int &height) = 0;
virtual void  getJointsPosition(jointListType& jointList) = 0;
virtual void  getRTMatrixList(RTMatrixList& RTMatList) = 0;
virtual bool saveFile(const string& text, const string& header) = 0;
virtual bool setJuego(const string& text) = 0;
virtual void  stopRecording() = 0;
virtual void  playVideo(const string& path) = 0;
virtual void  makeVideoVisible() = 0;
virtual void  makeARVisible() = 0;
virtual void  stopVideo() = 0;
virtual void  pauseVideo() = 0;
virtual void  playAr() = 0;
virtual void setGameRange(string game,float min,float max) = 0;
virtual void getGameRange(string game,float &min,float &max) = 0;
virtual void setSceneObjectPosition(const std::string& object, int x, int y,int z) = 0;
virtual void getSceneObjectPosition(const std::string& object,int &x,int &y,int &z) = 0;

protected:
	QTimer timer;
	int Period;
	//MapPrx proxies;
public slots:
	virtual void compute() = 0;
signals:
	void kill();
};

#endif