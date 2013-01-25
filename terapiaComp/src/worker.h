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
#ifndef WORKER_H
#define WORKER_H

#include <Armrocio.h>

// #include <Camera.h>

// #include <ipp.h>
#include <QtGui>
#include <stdint.h>

#include <DifferentialRobot.h>
#include <Speech.h>
#include <osgviewer/osgview.h>
#include <phonon/phonon>
#include "ui_terapiaDlg.h"
#include "ui_proyectorDlg.h"
#include "osgArt.h"
#include "cameraDlgControl.h"
#include <math.h>
#include <qvisor/qvisor.h>

#define CHECK_PERIOD 5000
#define BASIC_PERIOD 100

using namespace Ui;

class MiQDialog: public QDialog
{
Q_OBJECT
 public:
  MiQDialog():QDialog(){}
  MiQDialog(QWidget * p):QDialog(p){}
  ~MiQDialog(){}
  void	resizeEvent ( QResizeEvent * event) 
	  { emit resizeMiQDialog ();   }
  void	closeEvent ( QCloseEvent * event) 
	  { emit killMiQDialog(); } 

signals:
	  void resizeMiQDialog ( ); 
	  void killMiQDialog ( );
};
class MiQThread;


/**
       \brief
       @author authorname
*/
class Worker : public QMainWindow,  private Ui_MainWindow
{
Q_OBJECT
public:
	Worker(RoboCompArmrocio::ArmrocioPrx armrocioprx, RoboCompCamera::CameraPrx cameraprx, RoboCompDifferentialRobot::DifferentialRobotPrx differentialrobotprx, RoboCompSpeech::SpeechPrx speechprx, QObject *parent = 0);
	~Worker();
	//QMutex *mutex;                //Shared mutex with servant

	RoboCompDifferentialRobot::DifferentialRobotPrx differentialrobot;
	RoboCompSpeech::SpeechPrx speech;   
	RoboCompCamera::CameraPrx camera;	
	RoboCompArmrocio::ArmrocioPrx armrocio;
	
private:
	QTimer timer;	
	OsgArt *osgArt;
	OsgArt *osgArtDlg;
	MiQDialog dlg;

	qWorld *miVisor;
	QImage  *q;
	bool switchState;
	MiQThread *capturador;
	RoboCompJointMotor::MotorStateMap mstateMap;
	
	
	QGridLayout *gridLayout;
	QFrame *frameDlg;
	
	Phonon::MediaObject *mediaObject;
	Phonon::VideoWidget videoWidget;
	Phonon::AudioOutput *audioOutput;
	Phonon::VideoPlayer *videoPlayer2;
	Phonon::SeekSlider *seekSlider2;
	Phonon::VolumeSlider *volumeSlider2;
	
	QProcess *proccesWinfo, *proccesRecWin;
	
	QDir videosDir;
	QDir sentencesDir;
	QFile sentencesFile;
	QString absolutePath;
	Phonon::MediaObject *music;
	QHBoxLayout* hLayout;
	QVBoxLayout* vLayout;
	
	void fillVideoList();
	void fillSentenceList();
	void closeEvent( QCloseEvent * event); 
	void run();
	void updateTemperature( );
	
public slots:
  	void compute();
	void iniThread();
	void toTextEdit(QListWidgetItem*);
	void loadGame (QListWidgetItem* );
	void loadVideo(QListWidgetItem*);
	void recProyector( );
	
// 	void playPause();	
	void habla();
	void speechConseguido();
	void changeDirectory();
	void changeSentences();	
// 	void miSeek(qint64);
	void switchFrame();
 	void armHola();
	void armAdios();
};

class MiQThread: public QThread
{
// Q_OBJECT
 Worker *w;
 bool fin;
 public:
  MiQThread ( Worker * _w):QThread(){ w=_w; fin=false; }
  
  ~MiQThread(){}
  void	run ( ) 
	  {
	    while (not fin ) {
	      w->compute();
	      usleep(100000);      
	    }
	  
	  }
 void	stop ( ) 
	  { fin=true; } 

// signals:
// 	  void resizeMiQDialog ( ); 
// 	  void killMiQDialog ( );
};

#endif
