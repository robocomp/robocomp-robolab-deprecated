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
#include "aprendizI.h"

AprendizI::AprendizI(Worker *_worker, QObject *parent) : QObject(parent)
{
	worker = _worker;
	mutex = worker->mutex;       // Shared worker mutex
	// Component initialization...
}

void AprendizI::setWrist( const RoboCompAprendiz::coor3D& pose, const Ice::Current& )
{
	Vec3 v (pose.x,pose.y,pose.z);
	
	worker->setWrist(v);
}

void AprendizI::setElbowWrist(const RoboCompAprendiz::coor3D& poseElbow, const RoboCompAprendiz::coor3D& poseWrist,  const RoboCompAprendiz::matriz& m, const Ice::Current& )
{
	Vec3 elbow (poseElbow.x,poseElbow.y,poseElbow.z);
	Vec3 wrist (poseWrist.x,poseWrist.y,poseWrist.z);
	worker->setElbowWrist(wrist,elbow, m);

}

void AprendizI::getShoulders(coor3D& leftShoulder, coor3D& rightShoulder, const Ice::Current& )
{
	worker->getShoulders(leftShoulder,rightShoulder);
}

void AprendizI::getElbows(coor3D& leftElbow, coor3D& rightElbow, const Ice::Current& )
{
	worker->getElbows(leftElbow,rightElbow);
}


AprendizI::~AprendizI()
{
	// Free component resources here
}

// Component functions, implementation

// :-) @-@-@ INTERFACE FUNCTIONS IMPLEMENTATION  @-@-@ :-)

