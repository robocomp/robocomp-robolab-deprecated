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
#include <rcdraw/rcdraw.h>
#include "sesiondata.h"
#include <phonon/phonon>
#define MAX_TEMPERATURE 60


class MiQDialog: public QDialog
{
Q_OBJECT
public:
	MiQDialog():QDialog(){}
	MiQDialog(QWidget * p):QDialog(p){}
	~MiQDialog(){}
	void resizeEvent ( QResizeEvent * event){ emit resizeMiQDialog ();   }
	void closeEvent ( QCloseEvent * event)  { emit killMiQDialog(); } 
signals:
	void resizeMiQDialog ( ); 
	void killMiQDialog ( );
};



class SpecificWorker : public GenericWorker
{
Q_OBJECT
public:
	SpecificWorker(MapPrx& mprx);	
	~SpecificWorker();
	void setParams(RoboCompCommonBehavior::ParameterList params);
	void checkArms();
	void loadPhrases(QString file);
	
	
public slots:
	void saySlot();
	void compute();
	void toTextEdit(QListWidgetItem* item );
	void changeJuegoSlot(QListWidgetItem* item );
	void loadVideo(QListWidgetItem*);
	void changeDirectory();
	void changePhrases();
	void recordSlot1();
	void recordSlot0();
	void righMoveSlot();
	void leftMoveSlot();
	void myPlay();
	void myStop();
	void myPause();
	void videoToProyector();
	void arToProyector();
	void setVolume();
	void qtextEditChange();
	void changeGameRanges();
	void moveSceneObjectSlot();
	void updateObjectsSpinBoxPositions();
	
private: 
	QImage *miniFrame;
	
	sesionData *header;
	qint64 videoTime;
	
	QTimer timerRec;
	Phonon::MediaObject *mediaObject;
	Phonon::VideoWidget videoWidget;
	Phonon::AudioOutput *audioOutput;
	Phonon::MediaObject *music;
	
	QDir videosDir,pacientDir;
	QString absolutePath;
	
	
	RoboCompJuego::imgType img;
	int width,height;
	QImage *qimg;
	RCDraw *visor;
	bool leftArmBusy,rightArmBusy;

	
	RoboCompJointMotor::MotorStateMap motors;
};

#endif