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
#ifndef QRDECODEI_H
#define QRDECODEI_H

// QT includes
#include <QtCore/QObject>

// Ice includes
#include <Ice/Ice.h>
#include <Qrdecode.h>

#include <config.h>
#include "worker.h"

using namespace RoboCompQrdecode;

class QrdecodeI : public QObject , public virtual RoboCompQrdecode::Qrdecode
{
Q_OBJECT
public:
	QrdecodeI( Worker *_worker, QObject *parent = 0 );
	~QrdecodeI();

	// :-) @-@-@ INTERFACE FUNCTIONS DEFINITION  @-@-@ :-)

	std::string decode(const RoboCompQrdecode::imgType& img,int width,int height, const Ice::Current&);
	
	
	QMutex *mutex;
private:

	Worker *worker;
public slots:


};

#endif
