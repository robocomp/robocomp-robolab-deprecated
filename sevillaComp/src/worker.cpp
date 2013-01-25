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
#include "worker.h"
#include <QMessageBox>

Worker::Worker(RoboCompArmrocio::ArmrocioPrx armrocioprx, RoboCompCamera::CameraPrx cameraprx, RoboCompDifferentialRobot::DifferentialRobotPrx differentialrobotprx, RoboCompSpeech::SpeechPrx speechprx, QObject* parent) : Ui_MainWindow()
{
	armrocio = armrocioprx;
	camera = cameraprx;
	differentialrobot = differentialrobotprx;
	speech = speechprx;
	
	//hilo
	capturador = new MiQThread (this);
	
	proccesRecWin = NULL;
	proccesWinfo = NULL;
				
 	setupUi(this);	
	this->setWindowTitle("RobEx");
	juegosLW->item(0)->setSelected(true);
	dlg.resize(800,600);
	dlg.setWindowTitle("Proyector");
	pbRec->hide();
	
	
// 	connect(&timer, SIGNAL(timeout()), this, SLOT(compute()));
	connect (speechPB,SIGNAL(clicked()),this,SLOT(habla()));
	connect(openVidPB, SIGNAL(clicked()), this, SLOT(changeDirectory()));
	connect(openTextPB, SIGNAL(clicked()), this, SLOT(changeSentences()));
	connect (phrasesLW,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(toTextEdit(QListWidgetItem*)));
	connect (juegosLW,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(loadGame(QListWidgetItem*)));	
	connect (videosLW,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(loadVideo(QListWidgetItem*)));
	connect (pbRec,SIGNAL (pressed()), this, SLOT (recProyector()) );
	
// 	timer.start(BASIC_PERIOD);
	
	QIcon openIcon = style()->standardIcon(QStyle::SP_DialogOpenButton);
	openVidPB->setIcon(openIcon);
	openTextPB->setIcon(openIcon);

	QIcon playIcon = style()->standardIcon(QStyle::SP_MediaPlay);
	playPB->setIcon(playIcon);
	QIcon stopIcon = style()->standardIcon(QStyle::SP_MediaStop);
	stopPB->setIcon(stopIcon);
	
	
	 QStringList filters;
	filters << "*.avi" << "*.mpg"<<"*.wav"<<"*.ogg"<<"*.mov";
	videosDir.setNameFilters(filters);
	videosDir.setFilter(QDir::Files);
	
	hLayout = new QHBoxLayout(&dlg);	
	vLayout = new QVBoxLayout();	 

	videoPlayer2 = new Phonon::VideoPlayer(&dlg);
 	videoPlayer2->hide();
	vLayout->addWidget(videoPlayer2);

	///qframe con el video del camara para la interaccion con osgART	
	frameDlg= new QFrame ();
	frameDlg->setMinimumSize(800,600);
	frameDlg->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

	vLayout->addWidget(frameDlg);	
	hLayout->addLayout(vLayout);
	
	osgArt = new OsgArt(camera, frameDlg);    	
	q = new QImage (frame->size(),QImage::Format_RGB32 );
	miVisor = new qWorld (q,frame);
	dlg.show();
	show();
	
	switchState = false;
	switchPB->setText("Change to video");
	
	connect(playPB, SIGNAL(clicked()), videoPlayer, SLOT(play()));
	connect(switchPB, SIGNAL(clicked()), this, SLOT(switchFrame()));
	connect(playPB, SIGNAL(clicked()), videoPlayer2, SLOT(play()));
	connect(stopPB, SIGNAL(clicked()), videoPlayer2, SLOT(stop()));
 	connect( &dlg, SIGNAL (resizeMiQDialog()), osgArt, SLOT (resizeOsgArt()));
	connect( &dlg, SIGNAL (killMiQDialog()), osgArt, SLOT (closeOsgArt()));
	connect( osgArt, SIGNAL (signalSpeechConseguido()), this, SLOT (speechConseguido()) );
	connect( &dlg, SIGNAL (killMiQDialog()), this, SLOT (close()));
	connect(rbHola, SIGNAL (clicked()),this, SLOT (armHola()) );
	connect (rbAdios, SIGNAL (clicked()),this, SLOT (armAdios()) );
	// 	connect (videoPlayer->mediaObject(),SIGNAL (tick(qint64) ),this,SLOT (miSeek(qint64)) );
	// 	syncAdjust=0;
	
	//fichero de frases por defecto
	sentencesFile.setFileName("data/frasesBrazo.txt");
	this->fillSentenceList();
	
 	connect(&timer, SIGNAL(timeout()), this, SLOT(compute()));
//  	connect(&timer, SIGNAL(timeout()), this, SLOT(iniThread()));

	
 	timer.start(BASIC_PERIOD);
	
		
}

void Worker::iniThread()
{
//       capturador->start();
//       timer.stop();
//       disconnect(&timer, SIGNAL(timeout()), this, SLOT(iniThread()));
}  


// void Worker::playPause()
// {
//   Phonon::MediaObject *m_MediaObject; 
//   m_MediaObject= videoPlayer->mediaObject();  
//   
//   if (m_MediaObject->state() == Phonon::PlayingState)
// 	{     
// 	 playPB->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
// 	 m_MediaObject->pause();	 	 
// 	 
// 	}
//      else 
// 	 {
//        if (m_MediaObject->currentTime() == m_MediaObject->totalTime())
// 		{
// 			 playPB->setIcon(style()->standardIcon(QStyle::SP_MediaPlay)); 
//              m_MediaObject->seek(0);
// 	
// 		}	 
// 		playPB->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
//         m_MediaObject->play();	
//      }      
// }

void Worker::updateTemperature()
{
	
	try {
		armrocio->getJointState(mstateMap);
		//s1 hombro
		lcdHombro->display( mstateMap["s1"].temperature );
		//s3 pulso
		lcdPulso->display( mstateMap["s2"].temperature );
		//s4 codo
		lcdCodo->display( mstateMap["s3"].temperature );
	}
	catch (Ice::Exception e ) {
		qDebug()<<"Error talking to ArmRocio"<<e.what();
	}
}

void Worker::loadVideo(QListWidgetItem* item)
{      
  Phonon::MediaSource common (absolutePath+"/"+item->text());
  qDebug()<<absolutePath+"/"+item->text();
  //video player uno control
  videoPlayer->load(common);					  
  seekSlider->setMediaObject(videoPlayer->mediaObject()); 		  
  volumeSlider->setAudioOutput(videoPlayer->audioOutput());    
  
  //vidoe player 2
  videoPlayer2->load(common);  
  videoPlayer2->setVolume(0);
  playPB->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));  
}

void Worker::loadGame(QListWidgetItem* item)
{
	osgArt->setJuego(item->text(),fPanel, armrocio, speech); 
	if ( item->text() == QString::fromUtf8 ("Bienvenida / Despedida.") )	   
	{	
		rbHola->show();
		rbAdios->show();
		qDebug()<<"armrocio->setPose(RoboCompArmrocio::hola); ";		
// 		armrocio->setPose(RoboCompArmrocio::hola1);
	} 
	else
	{
		rbHola->hide();
		rbAdios->hide();
	}
}

void Worker::habla()
{
  try 
  {
	speech->say(textEdit->toPlainText().toStdString(),true);
  }
  catch (Ice::Exception e) 
  {
	qDebug()<<"Error talking to Speech: "<<e.what();
  }  
}

void Worker::toTextEdit(QListWidgetItem* item )
{   
	textEdit->setPlainText(item->text());
}

void Worker::changeDirectory()
{
	absolutePath.clear();
	absolutePath= QFileDialog::getExistingDirectory (this, tr("Directorio de vídeos"), videosDir.homePath());
	if ( absolutePath.isNull() == false )
	{	 
		videosDir.setPath(absolutePath);
		this->fillVideoList();
	}
}

void Worker::changeSentences()
{
	QString fileName= QFileDialog::getOpenFileName (this, tr("Fichero de frases"), sentencesDir.path());
	if (fileName.length() != 0)
	{
		sentencesDir.setPath(fileName);
		sentencesFile.setFileName(fileName);
		this->fillSentenceList();
	}
}

void Worker::fillVideoList()
{
	videosLW->clear();
	videosLW->addItems(videosDir.entryList());
}

void Worker::fillSentenceList()
{
	phrasesLW->clear();
	if (!sentencesFile.open(QIODevice::ReadOnly))
	{
		QString message;
		QTextStream(&message) << tr("Cannot read file ")<<sentencesFile.fileName()<<"\n"<<sentencesFile.errorString();
		QMessageBox::warning(this, tr("Application"), message);
		return;
	}
	QTextStream stream( &sentencesFile );
        while ( !stream.atEnd() )
	{
            phrasesLW->addItem(stream.readLine()); // line of text excluding '\n'
        }
        sentencesFile.close();
}

void Worker::switchFrame()
{
	
	if(switchState == false)
	{
		switchPB->setText("Change to AR");
		frameDlg->hide();
		videoPlayer2->show();		
	}
	else
	{
		switchPB->setText("Change to video");
		frameDlg->show();
		videoPlayer2->hide();
	}
	switchState=!switchState;
}
void Worker::speechConseguido()
{
	try 
	{
		QString s (QString::fromUtf8("<prosody volume=\"x-loud\"> ¡Conseguido!.</prosody> Cambiamos la bola. " ));
		speech->say( s.toStdString(),true);		
		
	}
	catch (Ice::Exception e) 
	{
		qDebug()<<"Error talking to Speech: "<<e.what();
	} 
}

void Worker::armHola()
{
  QTime t;
  int count=0;
  while(rbHola->isChecked())
  {
    if (count>1) 
      break;
    count++;
	try {
		armrocio->setMaxSpeed(6.f);
		armrocio->setPose(RoboCompArmrocio::hola1);
	}
	catch (Ice::Exception e ) {
		qDebug()<<"Error talking to Arm"<<e.what();
	}
	t.start();
	while(t.elapsed()<4000)
	{
	  if(!armrocio->isMoving())
	  {
	    break;
	  }
	  QApplication::processEvents();
	  usleep(1000);
	}
	if(count == 1)
	{
		try 
		{
			QString s (QString::fromUtf8("<prosody range=\"x-high\"> ¡Hola!.</prosody> <emphasis level=\"strong\">Es un placer verte.</emphasis>" ));	
			speech->say( s.toStdString(),true);		
		}
		catch (Ice::Exception e) 
		{
			qDebug()<<"Error talking to ArmRocio: "<<e.what();
		}
	}	
	try
	{
		armrocio->setPose(RoboCompArmrocio::hola2);
	}
	catch (Ice::Exception e ) {
		qDebug()<<"Error talking to Arm"<<e.what();
	}
	t.restart();
	while(t.elapsed()<4000)
	{
	  if(!armrocio->isMoving())
	  {
	    break;
	  }
	  QApplication::processEvents();
	  usleep(1000);
	}
  }
  try
  {
	  armrocio->setPose(RoboCompArmrocio::reposo);
	  armrocio->setMaxSpeed(2.5);
  }
  catch (Ice::Exception e ) {
	  qDebug()<<"Error talking to Arm"<<e.what();
  }
}

void Worker::armAdios()
{
	
	
 QTime t;
  int count=0;
  while(rbAdios->isChecked())
  {
    if (count>1) 
      break;
    count++;
	try {
		armrocio->setPose(RoboCompArmrocio::hola1);
	}
	catch (Ice::Exception e ) {
		qDebug()<<"Error talking to Arm"<<e.what();
	}
	t.start();
	while(t.elapsed()<4000)
	{
	  if(!armrocio->isMoving())
	  {
	    break;
	  }
	  QApplication::processEvents();
	  usleep(1000);
	}
	if(count == 1)
	{
	    try 
	    {
	    QString s (QString::fromUtf8("<prosody range=\"x-high\"> Adiós.</prosody> <emphasis level=\"moderate\"> Cuídate mucho. Nos vemos el próximo día.</emphasis>" ));	
	    speech->say( s.toStdString(),true);
    // 	armrocio->setPose(RoboCompArmrocio::adios);
	    }
	    catch (Ice::Exception e) 
	    {
		    qDebug()<<"Error talking to ArmRocio: "<<e.what();
	    } 
	}
	try
	{
		armrocio->setPose(RoboCompArmrocio::hola2);
	}
	catch (Ice::Exception e ) {
		qDebug()<<"Error talking to Arm"<<e.what();
	}
	t.restart();
	while(t.elapsed()<4000)
	{
	  if(!armrocio->isMoving())
	  {
	    break;
	  }
	  QApplication::processEvents();
	  usleep(1000);
	}
  }
  try
  {
	  armrocio->setPose(RoboCompArmrocio::reposo);
  }
  catch (Ice::Exception e ) {
	  qDebug()<<"Error talking to Arm"<<e.what();
  }
}

void Worker::recProyector()
{
	qDebug()<<"desayunar"<<pbRec->isDown();
	if ( pbRec->isDown() )
	{
		if (proccesRecWin != NULL ) {
			proccesRecWin->terminate();
			proccesRecWin->waitForFinished();
			proccesRecWin->kill();
			qDebug()<<"muerete";
			return;
		}
	}
	
	proccesWinfo = new QProcess();
	QString currentDir= QDir::currentPath();	
	qDebug()<<currentDir;
	proccesWinfo->setStandardOutputFile("info",QIODevice::WriteOnly);
	proccesWinfo->start("xwininfo -name Proyector",QIODevice::ReadWrite);
	proccesWinfo->waitForFinished();	
	
	QFile f ("info" );
	if (!f.open(QIODevice::ReadWrite))
         return;	
	
	QTextStream in (&f);				
	QString line, winId;
	QString patron="\""+ dlg.windowTitle() +"\"";
	do {
     line = in.readLine();
		if (line.contains(patron) ) {
			winId=line.section(':',2,2).split(patron).first();
			qDebug()<<winId;			
			break;
		}		 
	} while (!line.isNull());	
	f.close();	
	
	proccesRecWin = new QProcess();
	proccesRecWin->start("recordmydesktop --windowid "+ winId + " -o salida.ogv");	
	
}


Worker::~Worker()
{
	std::cout<<"destructor Sevilla";
	dlg.close();
 
}

void Worker::run()
{
    
      
}

void Worker::compute( )
{  
	osgArt->update();	
		memcpy (q->bits(),osgArt->grabFrameBuffer().scaled(frame->width(),frame->height()).bits(),frame->width() *frame->height()*4);	
	miVisor->update();
	updateTemperature();
}



void Worker::closeEvent( QCloseEvent * event)
{
	dlg.close();
	capturador->stop();
	capturador->wait();
}

