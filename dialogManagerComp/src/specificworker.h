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
#include <qstatemachinewidget/qstateMachineWrapper.h>
#include <QKeyEvent>
#include <macro_wait_state.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>

#define WAIT_TIME 5000
#define SPEECH_TIME 8000

/**
       \brief
       @author authorname
*/

class SpecificWorker : public GenericWorker
{
Q_OBJECT
private:
	
public:
	SpecificWorker(MapPrx& mprx, QWidget *parent = 0);	
	~SpecificWorker();
	void setParams(RoboCompCommonBehavior::ParameterList params);
	
	void initMachine();

private:
	QStateMachineWrapper* machine;
	QState *waitingState,*sayHelloState,*wAnswerHelloState,*sayWelcomeState,*sayNotUnderstandState,*wait02State,*say03State;
	QTimer timerMachine;
	bool face;
// 	QString persona, lugar;
	QSqlDatabase baseDatos;
	QSqlQuery consulta;
    bool foundLastname(QString person);
	QVector <QString> helloString, sayNotUnderstandString,say03String;

	
	RoboCompJointMotor::MotorGoalPositionList waiting,hi,notunderstand,notunderstand2,bye;
	

public slots:
 	void compute(); 	
	
	///metodos machine
	void waitingSlot();
	void sayHelloSlot();
	void wAnswerHelloStateSlot();
	void sayWelcomeSlot();
	void sayNotUnderstandSlot();
	void wait02Slot();
	void say03Slot();
signals:
	void faceDetected();
	void waitingAnswerSignal();
	void waitingBack();
	void notUnderstandSignal();
	void understandSignal();
	void wait02Signal();
	void say03Signal();
	
};

#endif