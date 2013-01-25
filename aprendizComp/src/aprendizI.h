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
#ifndef APRENDIZI_H
#define APRENDIZI_H

// QT includes
#include <QtCore/QObject>

// Ice includes
#include <Ice/Ice.h>
#include <Aprendiz.h>

#include <config.h>
#include "worker.h"

using namespace RoboCompAprendiz;

class AprendizI : public QObject , public virtual RoboCompAprendiz::Aprendiz
{
Q_OBJECT
public:
	AprendizI( Worker *_worker, QObject *parent = 0 );
	~AprendizI();

	// :-) @-@-@ INTERFACE FUNCTIONS DEFINITION  @-@-@ :-)
	void setWrist(const RoboCompAprendiz::coor3D &pose, const Ice::Current & = Ice::Current());
	void setElbowWrist(const RoboCompAprendiz::coor3D& poseElbow, const RoboCompAprendiz::coor3D& poseWrist, const RoboCompAprendiz::matriz& m, const Ice::Current& = Ice::Current());
	void getShoulders(RoboCompAprendiz::coor3D& leftShoulder, RoboCompAprendiz::coor3D& rightShoulder, const Ice::Current& = Ice::Current());
	void getElbows(RoboCompAprendiz::coor3D& leftElbow, RoboCompAprendiz::coor3D& rightElbow, const Ice::Current& = Ice::Current());

	QMutex *mutex;
private:

	Worker *worker;
public slots:


};

#endif
