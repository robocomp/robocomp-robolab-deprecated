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
#ifndef MOLEI_H
#define MOLEI_H

// QT includes
#include <QtCore/QObject>

// Ice includes
#include <Ice/Ice.h>
#include <Mole.h>

#include <config.h>
#include "worker.h"

using namespace RoboCompMole;

class MoleI : public QObject , public virtual RoboCompMole::Mole
{
Q_OBJECT
public:
	MoleI( Worker *_worker, QObject *parent = 0 );
	~MoleI();

	// :-) @-@-@ INTERFACE FUNCTIONS DEFINITION  @-@-@ :-)

	QMutex *mutex;
private:

	Worker *worker;
public slots:


};

#endif
