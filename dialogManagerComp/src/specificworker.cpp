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


SpecificWorker::SpecificWorker(MapPrx& mprx, QWidget  *parent) : GenericWorker(mprx)
{
	initMachine();
	face = false;
	
// 	qDebug()<<" codec"<<QTextCodec::codecForCStrings();
// 	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
// 	qDebug()<<" codec"<<QTextCodec::codecForCStrings();	
	
	helloString.append(QString::fromUtf8("Hola, ¿Cómo te llamas?"));
	helloString.append(QString::fromUtf8("Hola. Mi nombre es Muecas y mi apellido Robolab. ¿Cuáles son los tuyos?"));
	helloString.append(QString::fromUtf8("Bienvenido al congreso. ¿Me dices tu nombre y primer apellido?"));
	helloString.append(QString::fromUtf8("¡Hola! Me alegro de que ya hayas llegado. ¿Cómo te llamas?"));
	helloString.append(QString::fromUtf8("¡Hola! Soy Muecas, puedo entender tu nombre y tu primer apellido. ¿Me los dices?"));
	
	sayNotUnderstandString.append(QString::fromUtf8("Perdona no te he entendido. Acércate el micrófono y dime tu nombre y primer apellido, por favor."));
	sayNotUnderstandString.append(QString::fromUtf8("Lo siento. Acércate el micrófono. Puedes repetirme tu nombre y primer apellido, por favor."));
	sayNotUnderstandString.append(QString::fromUtf8("Perdona no me explicado bien, puedes acércarte el micrófono y decirme tu nombre y primer apellido, por favor."));
	sayNotUnderstandString.append(QString::fromUtf8("Perdona soy un poco duro de oído. Acércarte el micrófono y vuelve a decirme tu nombre y primer apellido, por favor."));
	
	say03String.append(QString::fromUtf8("Lo siento no te entiendo. Díselo a Ismael. Hasta la vista."));
	say03String.append(QString::fromUtf8("No soy capaz de entenderte. He fracasado. Habla con una persona, por favor."));
	say03String.append(QString::fromUtf8("Me temo que mis micrófonos son demasiado baratos. Perdóname no se que dices. Hasta pronto."));
	say03String.append(QString::fromUtf8("No te entiendo. Por favor, contacta con un humano. Nos vemos."));
	
	
// 	qFatal("adios q fatal");
	
	baseDatos = QSqlDatabase::addDatabase("QSQLITE");
	baseDatos.setDatabaseName("/home/robocomp/robocomp/Components/RoboLab/Experimental/dialogManagerComp/bin/waf.db");
	if (!baseDatos.open())
		qFatal("Base de Datos waf.db no pudo ser abierta");
	consulta = QSqlQuery (baseDatos);
	QSqlQuery gram = QSqlQuery (baseDatos);
	gram.exec("SELECT nombre, apellido1 FROM people");
	QFile file("/home/robocomp/robocomp/Components/RoboLab/Experimental/dialogManagerComp/bin/gram.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
         qFatal("Can not create File");
	QTextStream out(&file);
	out.setCodec(QTextCodec::codecForName("ISO-8859-1"));
	
	while (gram.next() ) {
		out<<gram.value(0).toString()<<" "<<gram.value(1).toString()<<"\n";
	}	
	file.close();
	
// 	QString s;
// 	foreach (s, helloString ) {
// 		qDebug()<<"qdebug() "<<s;
// 		std::cout<<s.toStdString()<<"\n";
// 	}
	
	show();
	connect(&timer, SIGNAL(timeout()),this,SLOT(compute()));
	timer.start(500);
	
	//EyeBrown positions 
	RoboCompJointMotor::MotorGoalPosition eLUp,eLYaw,eRUp,eRYaw;
	eLUp.name = "eyeBLeftUP";
	eLYaw.name = "eyeBLeftYAW";
	eRUp.name = "eyeBRightUP";
	eRYaw.name = "eyeBRightYAW";	
	
	//waiting
	eLUp.position = 0.f;
	eLYaw.position = 0.f;
	eRUp.position = 0.f;
	eLYaw.position = 0.f;
	waiting.push_back(eLUp);
	waiting.push_back(eLYaw);
	waiting.push_back(eRUp);
	waiting.push_back(eRYaw);
	//hi
	eLUp.position = 0.4;
	eLYaw.position = 0.4;
	eRUp.position = 0.3;
	eLYaw.position = -0.3;
	hi.push_back(eLUp);
	hi.push_back(eLYaw);
	hi.push_back(eRUp);
	hi.push_back(eRYaw);
	//notunderstand
	eLUp.position = -0.4;
	eLYaw.position = 0.6;
	eRUp.position = 0.f;
	eLYaw.position = 0.f;
	notunderstand.push_back(eLUp);
	notunderstand.push_back(eLYaw);
	notunderstand.push_back(eRUp);
	notunderstand.push_back(eRYaw);
	//notunderstand2
	eLUp.position = -0.4;
	eLYaw.position = 0.6;
	eRUp.position = -0.2;
	eLYaw.position = -0.2;
	notunderstand2.push_back(eLUp);
	notunderstand2.push_back(eLYaw);
	notunderstand2.push_back(eRUp);
	notunderstand2.push_back(eRYaw);
	
	try{
	  jointmotor->setSyncPosition(waiting);
	}
	catch(Ice::Exception e)
	{
	  std::cout<<e.what()<<std::endl;
	}
	
	
	
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{

}
void SpecificWorker::compute( )
{
// 	qDebug()<<this->faceButton->isDown();
}
void SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	timer.start(Period);
}


void SpecificWorker::initMachine()
{
	machine = new QStateMachineWrapper();
	
	//states
	waitingState = machine->addState("waiting");
	machine->setInitialState(waitingState);
	
	sayHelloState = machine->addState("Hello");
	wAnswerHelloState = machine->addState("wAnswerHelloState");
	sayWelcomeState= machine->addState("Welcome");
	sayNotUnderstandState = machine->addState("Not Understand");
	wait02State=machine->addState("Wait 02");
	say03State = machine->addState("Say 03");
	
	machine->addTransition(waitingState,&timerMachine,SIGNAL(timeout() ), waitingState);
	machine->addTransition(waitingState,this,SIGNAL(faceDetected() ), sayHelloState);
	
// 	machine->addTransition(wAnswerHelloState,&timerMachine,SIGNAL(timeout() ),wAnswerHelloState );
	machine->addTransition(sayHelloState,this,SIGNAL(waitingAnswerSignal() ), wAnswerHelloState);
	
	///estado doble	
	machine->addTransition(wAnswerHelloState,this,SIGNAL(understandSignal() ), sayWelcomeState);
	machine->addTransition(wAnswerHelloState,this,SIGNAL(notUnderstandSignal() ), sayNotUnderstandState);

	machine->addTransition(sayNotUnderstandState,this,SIGNAL(wait02Signal() ),wait02State);
	
	///estado doble
	machine->addTransition(wait02State,this,SIGNAL(understandSignal()),sayWelcomeState);
	machine->addTransition(wait02State,this,SIGNAL(say03Signal()),say03State);
	
	///vuelvo al principio
	machine->addTransition(sayWelcomeState,this,SIGNAL(waitingBack() ),waitingState);
	machine->addTransition(say03State,this,SIGNAL(waitingBack() ),waitingState);
	
	
	//cuando se entra a un estado hay que hacer su slot.
	connect(waitingState,SIGNAL(entered()),this,SLOT(waitingSlot()));
	connect(sayHelloState,SIGNAL(entered()),this,SLOT(sayHelloSlot()));	
	connect(wAnswerHelloState,SIGNAL(entered()),this,SLOT(wAnswerHelloStateSlot()));		
	
	connect(sayWelcomeState,SIGNAL(entered()),this,SLOT(sayWelcomeSlot()));
	connect(sayNotUnderstandState,SIGNAL(entered()),this,SLOT(sayNotUnderstandSlot()));
	
	connect(wait02State,SIGNAL(entered()),this,SLOT(wait02Slot()));
	connect(say03State,SIGNAL(entered()),this,SLOT(say03Slot()));
	
	machine->start();
	
	timerMachine.start(Period);
}
void SpecificWorker::waitingSlot()
{
// 		qDebug()<<"face ?"<<face;				
//		if ( this->faceButton->isChecked() ) {
 //			emit faceDetected();
 //			this->faceButton->setChecked(false);
 //		}
		if ( face == true ) {
			sleep(5);
			face = false;
		}
		
		try 
		{
			if (facedetector->getState() == RoboCompFaceDetector::NearFace)
			{
				face = true;
				qDebug()<<"face ?"<<face;		
				emit faceDetected();
				
			}
		}
		catch (const Ice::Exception &e )
		{
			qFatal("ERROR talking to FACEDETECTOR ");
		}
}
void SpecificWorker::sayHelloSlot()
{
	qDebug()<<"say hello Slot";
	try{
	    jointmotor->setSyncPosition(hi);
	}
	catch(Ice::Exception e)
	{
	    std::cout<<"Error talking to joint (hi)"<<e.what()<<std::endl;
	}
	try {		
		
		string m = helloString.at(qrand()%helloString.size()).toStdString();
		cout<<m<<"\n";
		speech->say(m,true);
		
	}
	catch (const Ice::Exception &e )
	{
		qDebug()<<"QFATAL: ERROR talking to SPEECH";		
	}
	
	try
	{ 
		waitForFalseValue(speech->isBusy, SPEECH_TIME ); 				
	}
	catch(int r){ qDebug() << "catch sayHelloSlot waitForFalseValue(speech->isBusy() " << r;}
	emit waitingAnswerSignal();
}

void SpecificWorker::wAnswerHelloStateSlot()
{
	qDebug()<<"wAnswerHelloStateSlot()";
	try
	{
		asr->resetPhraseBuffer();		
	}
	catch (const Ice::Exception &e )
	{
		qFatal("QFATAL: ERROR talking to ASR. ");		
	}	
	bool waitTry=false;
	try
	{ 
		waitForTrueValue(asr->phraseAvailable, WAIT_TIME ); 		
		waitTry = true;
	}
	catch(int r)
	{ 
		qDebug() << "catch wAnswerHelloStateSlot not understand" << r;
	}
	catch (const Ice::Exception &e )
	{
		qFatal("QFATAL: ERROR talking to ASR. ");		
	}	
	if ( waitTry == true )
	{
		try
		{
			if (foundLastname(QString::fromStdString(asr->getLastPhrase())) )
			{
				qDebug()<<"-- matching"<<consulta.value(0).toString();
				emit understandSignal();
			}
			else
				emit notUnderstandSignal();
		}
		catch (const Ice::Exception &e )
		{
		qFatal("QFATAL: ERROR talking to ASR. ");		
		}	
	}
	else
	{
		emit notUnderstandSignal();
	}
}

void SpecificWorker::sayWelcomeSlot()
{
	qDebug()<<"sayWelcomeSlot";

	try {				
		QString localidad= consulta.record().field("localidad").value().toString();
		QString universidad= consulta.record().field("universidad").value().toString();
		QString nombre= consulta.record().field("nombre").value().toString();
		QString say = "Espero que hayas venido bien desde " +localidad+". Recoge tu inscripción" + nombre +". Y accede a la sala.";
		speech->say(say.toStdString(),true);
	}
	catch (const Ice::Exception &e )
	{
		qFatal("QFATAL: ERROR talking to SPEECH");		
	}
	
	try
	{ 
		waitForFalseValue(speech->isBusy, SPEECH_TIME ); 		
		qDebug()<<"fin dialogo. Vuelta al principio";	
	}
	catch(int r){ qDebug() << "catch sayWelcomeSlot waitForFalseValue(speech->isBusy() " << r;}
	emit waitingBack();
}

void SpecificWorker::sayNotUnderstandSlot()
{
	qDebug()<<"sayNotUnderstandSlot";
	try{
		jointmotor->setSyncPosition(notunderstand);
	}
	catch(Ice::Exception e)
	{
		  std::cout<<"Error talking to joint (not understand)"<<e.what()<<std::endl;
	}
	try 
	{
		
		speech->say(sayNotUnderstandString.at(qrand()%sayNotUnderstandString.size()).toStdString(),true);
	}
	catch (const Ice::Exception &e )
	{
		qFatal("QFATAL: ERROR talking to SPEECH");		
	}
	
	try
	{ 
		waitForFalseValue(speech->isBusy, SPEECH_TIME); 				
	}
	catch(int r){ qDebug() << "catch sayNotUnderstandSlot waitForFalseValue(speech->isBusy() " << r;}
	emit wait02Signal();
}

void SpecificWorker::wait02Slot()
{
	qDebug()<<"wait02Slot";
	try
	{
		asr->resetPhraseBuffer();		
	}
	catch (const Ice::Exception &e )
	{
		qFatal("QFATAL: ERROR talking to ASR. ");		
	}	
	
	bool waitTry=false;
	try
	{ 
		waitForTrueValue(asr->phraseAvailable, WAIT_TIME); 
		waitTry=true;
	}
	 catch(int r)
        { 
                qDebug() << "catch w02Slot not understand" << r;
        }
	catch (const Ice::Exception &e )
	{
		qFatal("QFATAL: ERROR talking to ASR. ");		
	}	
	if (waitTry)
	{
		try
		{
			if (foundLastname(QString::fromStdString(asr->getLastPhrase())) )
			{
				qDebug()<<"matching"<<consulta.value(0).toString();
				emit understandSignal();
			}
			else				
				emit say03Signal();
		}
		catch (const Ice::Exception &e )
		{
			qFatal("QFATAL: ERROR talking to ASR. ");		
		}	
	}
	else
		emit say03Signal();
	
}
void SpecificWorker::say03Slot()
{
  	try{
		jointmotor->setSyncPosition(notunderstand2);
	}
	catch(Ice::Exception e)
	{
		  std::cout<<"Error talking to joint (notunderstand2)"<<e.what()<<std::endl;
	}
	try {		
		
		speech->say(say03String.at(qrand()%say03String.size()).toStdString(),true);
	}
	catch (const Ice::Exception &e )
	{
		qFatal("QFATAL: ERROR talking to SPEECH");		
	}	
	try
	{ 
		waitForFalseValue(speech->isBusy, SPEECH_TIME); 				
		
	}
	catch(int r){ qDebug() << "catch sayNotUnderstandSlot waitForFalseValue(speech->isBusy() " << r;}
	emit waitingBack();
}

bool SpecificWorker::foundLastname(QString person)
{
	QString nombre =person.split(" ").first();
	QString apellido =person.split(" ").last();
	consulta.exec("SELECT localidad, universidad, nombre FROM people WHERE apellido1 = '"+apellido+"'");
	qDebug()<<"*****************"<<person<<"split"<<nombre<<apellido <<consulta.lastError().text();
	if (consulta.next() )
		return true;
	else
		return false;
}






