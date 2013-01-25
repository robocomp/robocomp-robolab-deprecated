/****************************************************************************
#                                                                           #
#                     Copyright (C) 2010                                    #
#                                                                           #
# This program is free software; you can redistribute it and/or modify      #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation; either version 2 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# This program is distributed in the hope that it will be useful,           #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with this program; if not, write to the Free Software               #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA #
#                                                                           #
****************************************************************************/

#ifndef WORKER_H
#define WORKER_H

// #include <ipp.h>
#include <QtGui>
#include <stdint.h>
#include <QList>

#include <Camera.h>
#include <DifferentialRobot.h>
#include <JointMotor.h>
#include <Cxtrelan.h>
#include <innermodel/innermodel.h>
#include <QMat/qvec.h>
#define CHECK_PERIOD 5000
#define BASIC_PERIOD 100


/**
       \brief
       @author authorname
*/
class Worker : public QObject
{
Q_OBJECT
public:
	Worker(RoboCompCamera::CameraPrx cameraprx, RoboCompDifferentialRobot::DifferentialRobotPrx differentialrobotprx,RoboCompJointMotor::JointMotorPrx jointmotorprx, QObject *parent = 0);
	~Worker();
	void imagenReal(RoboCompCamera::imgType &real);
	void imagenBinaria(RoboCompCamera::imgType &binaria);
	void mover(float avance, float giro);
	void vectorPuntos(RoboCompCxtrelan::ListaPuntos &lp);
	void vectorPuntosImagen(RoboCompCxtrelan::ListaPuntos &lp);
	void setThreshold(int threshold,bool tipo);

	
	int threshold;
	bool tipo;
	
	float tilt;
	float leftpan;
	//QMutex *mutex;                //Shared mutex with servant

	RoboCompCamera::CameraPrx camera;
	RoboCompDifferentialRobot::DifferentialRobotPrx differentialrobot;
	RoboCompJointMotor::JointMotorPrx jointmotor;
	RoboCompCamera::imgType imgReal;
 	RoboCompCamera::imgType imgBinaria;
	RoboCompCamMotion::THeadState hState;
	RoboCompDifferentialRobot::TBaseState bState;
	RoboCompCxtrelan::punto p;
	RoboCompCxtrelan::ListaPuntos vectorP;
	RoboCompCxtrelan::ListaPuntos vectorPI;
	InnerModel *innerModel;

private:
	QTimer timer;

public slots:
	void compute();
};

#endif
