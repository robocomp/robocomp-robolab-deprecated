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
#include "cxtrelanI.h"

CxtrelanI::CxtrelanI(Worker *_worker, QObject *parent) : QObject(parent)
{
	worker = _worker;
	//mutex = worker->mutex;       // Shared worker mutex
	// Component initialization...
}


CxtrelanI::~CxtrelanI()
{
	// Free component resources here
}

// Component functions, implementation

void CxtrelanI::imagenReal(RoboCompCamera::imgType &real, const Ice::Current &)
{
  worker->imagenReal(real);
}

void CxtrelanI::imagenBinaria(RoboCompCamera::imgType &binaria, const Ice::Current &)
{
  worker->imagenBinaria(binaria);
}

void CxtrelanI::mover(float avance, float giro, const Ice::Current &)
{
  worker->mover(avance, giro);
}

void CxtrelanI::vectorPuntos(RoboCompCxtrelan::ListaPuntos &lp, const Ice::Current &)
{
 worker->vectorPuntos(lp); 
}

void CxtrelanI::vectorPuntosImagen(RoboCompCxtrelan::ListaPuntos &lp, const Ice::Current &)
{
 worker->vectorPuntosImagen(lp); 
}
void CxtrelanI::setThreshold(int threshold,bool tipo, const Ice::Current &)
{
	worker->setThreshold(threshold,tipo);

}
