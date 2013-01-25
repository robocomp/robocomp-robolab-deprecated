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
// #include "imagestreamcallback.h"
/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)	
{
	videoTime = 0;
	width = 800;
	height = 600 ;
	miniFrame = new QImage(width, height,QImage::Format_RGB32);
	
	//Phrases
	connect (phrasesLW,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(toTextEdit(QListWidgetItem*)));
	connect (openTextPB,SIGNAL(clicked()),this,SLOT(changePhrases()));
	connect (speechPB,SIGNAL(clicked()),this,SLOT(saySlot()));
	connect (textEdit,SIGNAL(textChanged ()),this,SLOT(qtextEditChange()));
	
	//Reproductor video
	openVidPB->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
	openTextPB->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
	playPB->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	stopPB->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
	pausePB->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
	
	connect(videosLW,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(loadVideo(QListWidgetItem*)));
	connect(playPB, SIGNAL(clicked()), this, SLOT(myPlay()));
	connect(stopPB, SIGNAL(clicked()), this, SLOT(myStop()));
 	connect(pausePB,SIGNAL(clicked(bool)),this,SLOT(myPause()));
	
	connect(arRadio,SIGNAL(clicked(bool)),this,SLOT(arToProyector()));
	connect(videoRadio,SIGNAL(clicked(bool)),this,SLOT(videoToProyector()));
	connect(volumeSlider,SIGNAL(setAudioOutput(Phonon::AudioOutput*)),this,SLOT(setVolume()));
	
	//Videos
	QStringList filters;
	filters << "*.avi" << "*.mpg"<<"*.wav"<<"*.ogg"<<"*.mov";
	videosDir.setNameFilters(filters);
	videosDir.setFilter(QDir::Files);
	connect(openVidPB, SIGNAL(clicked()), this, SLOT(changeDirectory()));
	

	qimg = new QImage(frame->width(),frame->height(),QImage::Format_RGB32);
	visor = new RCDraw(frame->width(),frame->height(),qimg,frame);

	//Record file
	QStringList filters2;
	filters2 << "*.txt";
	pacientDir.setNameFilters(filters2);
	pacientDir.setFilter(QDir::Files);
	connect(pbRec,SIGNAL(clicked()),this,SLOT(recordSlot0()));
	
	//Ursus connect
	connect(rightMove,SIGNAL(clicked()),this,SLOT(righMoveSlot()));
	connect(leftMove,SIGNAL(clicked()),this,SLOT(leftMoveSlot()));
	leftCB->addItem("Hombro");	leftCB->addItem("Codo");
	leftCB->addItem("Reposo");	leftCB->addItem("Hola");
	rightCB->addItem("Hombro");	rightCB->addItem("Codo");
	rightCB->addItem("Reposo");	rightCB->addItem("Hola");
	
	//Slider
	connect(minSlider,SIGNAL(valueChanged(int)),this,SLOT(changeGameRanges()));
	connect(maxSlider,SIGNAL(valueChanged(int)),this,SLOT(changeGameRanges()));
	
	//Juegos
	connect (juegosLW,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(changeJuegoSlot(QListWidgetItem*)));
	connect (sbX,SIGNAL(valueChanged(int)),this,SLOT(moveSceneObjectSlot()));
	connect (sbY,SIGNAL(valueChanged(int)),this,SLOT(moveSceneObjectSlot()));
	connect (sbZ,SIGNAL(valueChanged(int)),this,SLOT(moveSceneObjectSlot()));
	connect (cbObject,SIGNAL(currentIndexChanged(int)),this,SLOT(updateObjectsSpinBoxPositions()));
	
	timer.start(Period);
	
	arToProyector();
qDebug()<<"fin constructor";
	show();
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	std::cout<<"destructor Sevilla";
}
void SpecificWorker::compute( )
{
	
	//Pericion imágenes al trackerOpenNi
	try
	{
		juego->getARimg(img,width,height);		
// 		qDebug()<<img.size()<<frame->width()<<frame->height();
		QImage a =miniFrame->scaled(width,height);
		memcpy(a.bits(),&img[0],width*height*4);
		memcpy(qimg->bits(),a.scaled(frame->width(),frame->height()).bits(),frame->width()*frame->height()*4);
	}
	catch(const Ice::Exception &e)
	{
		qDebug()<<"Error reading images from openNi"<<e.what();
	}
	checkArms();


	visor->update();
}
void SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	timer.start(Period);
}


void SpecificWorker::checkArms()
{
	//Movements
	try
	{
		leftArmBusy = ursusarm->isBusy("left");
		rightArmBusy = ursusarm->isBusy("right");
	}
	catch(const Ice::Exception &e)
	{
		qDebug()<<"Read ursusArm state"<<e.what();
	}
	if(leftArmBusy)
		leftBusy->setChecked(true);
	else
	{
		leftBusy->setChecked(false);
		leftMove->setChecked(false);
	}
	if(rightArmBusy)
		rightBusy->setChecked(true);
	else
	{
		rightBusy->setChecked(false);
		rightMove->setChecked(false);
	}
	//Temperatures
	try
	{
		ursusarm->getAllMotorState(motors);
	}
	catch(const Ice::Exception &e)
	{
		qDebug()<<"Read ursusArm motors states"<<e.what();
	}
	QString warning;
	map<std::string,RoboCompJointMotor::MotorState>::iterator it;
	for(it = motors.begin(); it!= motors.end(); it++)
	{
		if(it->second.temperature > MAX_TEMPERATURE)
			warning += QString::fromStdString(it->first)+"("+QString::number(it->second.temperature)+") "; 
	}
	if(warning != "")
		warningLabel->setText("Overheat: "+warning);
	else
		warningLabel->setText("");
}



void SpecificWorker::loadPhrases(QString path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Can't open" <<file.fileName();
		return;
	}
	QTextStream in(&file);
	while(!in.atEnd())
	{
		phrasesLW->addItem(in.readLine());
	}
}


//Interfaz events => SLOTS
void SpecificWorker::loadVideo(QListWidgetItem* item)
{      
	Phonon::MediaSource common (absolutePath+"/"+item->text());
	qDebug()<<absolutePath+"/"+item->text();
	videoPlayer->load(common);  
	seekSlider->setMediaObject(videoPlayer->mediaObject());
	volumeSlider->setAudioOutput(videoPlayer->audioOutput());    
	playPB->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	juego->stopVideo();
}


void SpecificWorker::toTextEdit(QListWidgetItem* item )
{   
	textEdit->setPlainText(item->text());
}
void SpecificWorker::changeJuegoSlot(QListWidgetItem* item )
{
	QString nameUtf8 =item->text().toUtf8();
	qDebug()<<item->text();
	try 
	{
		juego->setJuego(nameUtf8.toStdString());
	}
	catch (Ice::Exception e) 
	{
		qDebug()<<"Error setting juego: "<<e.what();
	}
	try 
	{
		float min = 0.f, max = 0.f;
		juego->getGameRange(nameUtf8.toStdString(),min,max);
		minSlider->setValue(min);
		maxSlider->setValue(max);
	}
	catch (Ice::Exception e) 
	{
		qDebug()<<"Error setting juego ranges: "<<e.what();
	}
// 	updateObjectsSpinBoxPositions();
}

void SpecificWorker::changeGameRanges()
{
	QString nameUtf8 = juegosLW->currentItem()->text().toUtf8();
	try 
	{
		juego->setGameRange(nameUtf8.toStdString(),minSlider->value(),maxSlider->value());
	}
	catch (Ice::Exception e) 
	{
		qDebug()<<"Error setting juego ranges: "<<e.what();
	}
}


void SpecificWorker::changeDirectory()
{
	absolutePath.clear();
	absolutePath = QFileDialog::getExistingDirectory (this, tr("Directorio de videos"), QDir::homePath());
	if ( absolutePath.isNull() == false )
	{
		videosDir.setPath(absolutePath);
		videosLW->clear();
		videosLW->addItems(videosDir.entryList());
	}
}
void SpecificWorker::changePhrases()
{
	QString path = QFileDialog::getOpenFileName (this, tr("Fichero de frases"),QDir::homePath(),tr("Texto (*.txt)"));
	if ( path.isNull() == false )
	{
		phrasesLW->clear();
		loadPhrases(path);
	}
}
void SpecificWorker::saySlot()
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

void SpecificWorker::recordSlot0()
{
	if (pbRec->text().compare("Record")==0)
	{
		header = new sesionData();
		connect(header, SIGNAL(done()), this, SLOT(recordSlot1()));
		pbRec->setText("Stop");
	}
	else
	{
		try
		{
			juego->stopRecording();	  
		}
		catch(const Ice::Exception &e)
		{
			qDebug()<<"Set file path to juego"<<e.what();
		}
		pbRec->setText("Record");
	}
}

void SpecificWorker::qtextEditChange()
{
	if(textEdit->toPlainText().size() >= 1 and textEdit->toPlainText()[textEdit->toPlainText().size()-1] == '\n')
	{
		try 
		{
			speech->say(textEdit->toPlainText().toStdString(),true);
		}
		catch (Ice::Exception e) 
		{
			qDebug()<<"Error talking to Speech: "<<e.what();
		}
		textEdit->clear();
	}
}
void SpecificWorker::recordSlot1()
{
	QString fileHeader;
  
	fileHeader = header->getHeader();
	header->close();
	disconnect(header, SIGNAL(done()), this, SLOT(recorSlot1()));
	delete(header);
	QString path =QFileDialog::getSaveFileName (this, tr("Fichero a salvar"), QDir::homePath());
	pacientDir.setPath(path);
	
	
	if(path.compare("")==0)
		return;
	
	try
	{
		path= path.toUtf8();
		juego->saveFile(path.toStdString(),fileHeader.toStdString());
	}
	catch(const Ice::Exception &e)
	{
		qDebug()<<"Set file path to juego"<<e.what();
	}
}


void SpecificWorker::leftMoveSlot()
{
	if(leftMove->isChecked())
	{
		if(!leftArmBusy)
		{
			try
			{
				ursusarm->loadMovement(QString(leftCB->currentText()+"Izq.txt").toStdString(),"left");
				ursusarm->reloadMovement("left");
			}
			catch(const Ice::Exception &e)
			{
				qDebug()<<"Error setting move to leftArm"<<e.what();
			}
		}
		else
			leftMove->setChecked(false);
	}
}
void SpecificWorker::righMoveSlot()
{
	if(rightMove->isChecked())
	{
		if(!rightArmBusy)
		{
			try
			{
				ursusarm->loadMovement(QString(rightCB->currentText()+"Der.txt").toStdString(),"right");
				ursusarm->reloadMovement("right");
			}
			catch(const Ice::Exception &e)
			{
				qDebug()<<"Error setting move to rightArm"<<e.what();
			}
		}
		else
			rightMove->setChecked(false);
	}
}


void SpecificWorker::myPlay()
{
	QString path;
	if(videosLW->count() <= 0)
		return;
  
	qDebug()<<videosLW->currentItem()<<" **************************"    ;
	if(videosLW->selectedItems().size() <= 0)
		return;
  
	path = absolutePath+"/"+videosLW->currentItem()->text();
	videoPlayer->load(path);
  
  //if(videoRadio->isChecked())
//falta try
    //juego->playVideo(path.toStdString());
  /*
  if(videoPlayer->isPaused()){
      videoPlayer->seek(videoTime);
      if(videoRadio->isChecked());
	///Seek video en juego*/
  //}
	videoPlayer->play();
	if(videoRadio->isChecked())
	{
		try
		{
			juego->playVideo(path.toStdString());
		}
		catch(Ice::Exception &e)
		{
			qDebug()<<"Error talking to juego"<<e.what();
		}
	}
}

void SpecificWorker::myPause()
{
	if(videoPlayer->isPlaying())
	{
		videoTime = videoPlayer->currentTime();
		videoPlayer->pause();
		if(videoRadio->isChecked())
		{
			try
			{
				juego->stopVideo();    
			}
			catch(Ice::Exception &e){
				qDebug()<<"Error talking to juego. juego->playAR"<<e.what();
			}
		}
	}
}

void SpecificWorker::myStop()
{
	if(videoPlayer->isPaused() || videoPlayer->isPlaying())
	{
		videoPlayer->stop();
		videoTime = 0;
		if(videoRadio->isChecked())
		{
			try
			{
				juego->stopVideo();    
			}
			catch(Ice::Exception &e){
				qDebug()<<"Error talking to juego. juego->playAR"<<e.what();
			}
		}
	}
}


void SpecificWorker::arToProyector()
{
	try
	{
		juego->makeARVisible();
	}
	catch(Ice::Exception &e){
		qDebug()<<"Error talking to juego. juego->makeARVisible()"<<e.what();
	}
}

void SpecificWorker::videoToProyector()
{
	QString path;
	try
	{
		juego->makeVideoVisible();
		if(videoPlayer->isPlaying())
		{
			path = absolutePath+"/"+videosLW->currentItem()->text();
			juego->playVideo(path.toStdString());
			videoPlayer->play(path);  
		}
	}
	catch(Ice::Exception &e){
		qDebug()<<"Error al reproducir Video "<<path<<" en juego";
	}
}

void SpecificWorker::setVolume()
{

}

void SpecificWorker::moveSceneObjectSlot()
{
	std::string object="";
	if(cbObject->currentText() == "Cesta")
		object = "XN_CESTA";
	else if(cbObject->currentText() == "Arbol")
		object = "XN_ESCENARIO_ARBOL";
	else if(cbObject->currentText() == "Torso")
		object = "XN_SKEL_TORSO";
	else if(cbObject->currentText() == "Manzana"){		
		object = "XN_MANZANA_ANIMADA";		
	}
	try
	{
		juego->setSceneObjectPosition(object,sbX->value(),sbY->value(),sbZ->value());
	}
	catch(Ice::Exception &e){
		qDebug()<<"Error trying to move scene object"<<e.what();
	}
}

void SpecificWorker::updateObjectsSpinBoxPositions()
{
	int x=0,y=0,z=0;
	std::string object="";
	if(cbObject->currentText() == "Cesta")
		object = "XN_CESTA";
	else if(cbObject->currentText() == "Arbol")
		object = "XN_ESCENARIO_ARBOL";
	else if(cbObject->currentText() == "Torso")
		object = "XN_SKEL_TORSO";
	else if(cbObject->currentText() == "Manzana"){		
		object = "XN_MANZANA_ANIMADA";		
	}
	try
	{
		juego->getSceneObjectPosition(object,x,y,z);
		sbX->setValue(x);sbY->setValue(y);sbZ->setValue(z);
	}
	catch(Ice::Exception &e){
		qDebug()<<"Error reading scene objects positions"<<e.what();
	}
  
}
