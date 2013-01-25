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

SpecificWorker::SpecificWorker(MapPrx& mprx, QThread *parent) : GenericWorker(mprx, parent)	
{
	talkToBase = false;
	lDataMutex = new QMutex();
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	uninitialize();
}
void SpecificWorker::initialize()
{
	sickld = new SickLD(laserConf.device);
	
	try{
		sickld->Initialize();
	}
	catch(...) {
		qFatal("Initialize failed! Are you using the correct IP address?");
	}
	double init = (double)laserConf.iniRange;
	double end = (double)laserConf.endRange;

	/* Set the desired sector configuration */
    sickld->SetSickTempScanAreas(&init,&end,laserConf.angleRes);
    
    /* Print the sector configuration */
    sickld->PrintSickSectorConfig();
}

void SpecificWorker::uninitialize()
{
	try{
		sickld->Uninitialize();
	}
	catch(...){
		qDebug()<<"Uninitialize failed!";
	}
}
void SpecificWorker::run()
{
	if(!talkToBase)
	{
		forever
		{
			if (readLaserData()==false)
			{
				rError("Error reading laser ");
			}
			usleep(Period);
		}
	}
	else{
		forever
		{
			try
			{
				differentialrobot->getBaseState(bState);
			}
			catch(Ice::Exception e)
			{
				rError("Error talking to differentialRobot");
			}
			if (readLaserData()==false)
			{
				rError("Error reading laser ");
			}
			usleep(Period);
		}
	}
}
bool SpecificWorker::readLaserData()
{
	RoboCompLaser::TData data;
	try 
	{
		sickld->GetSickMeasurements(polarvalues,NULL,&num_values);
		lDataMutex->lock();
			laserData.clear();
			for(unsigned int i = 0; i < num_values; i++)
			{
				laserPolarData[i] = polarvalues[i];
				data.angle = (laserConf.angleIni + i * laserConf.angleRes - 180.f)*M_PI/180.f;
				data.dist = polarvalues[i]*1000;
				laserData.push_back(data);
			}
		lDataMutex->unlock();
	}
    // Catch possible exceptions
  	catch(...) 
	{
		rError("Error while receiving laser data");
		return false;
  	}  
  	return true;
};

void SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	
	laserConf.device = params["device"].value;
	laserConf.iniRange = QString::fromStdString(params["startValue"].value).toInt();
	laserConf.endRange = QString::fromStdString(params["endValue"].value).toInt();
	laserConf.sampleRate = QString::fromStdString(params["sampleRate"].value).toInt();
	laserConf.angleRes = QString::fromStdString(params["angleRes"].value).toInt();
	talkToBase = QString::fromStdString(params["talkToBase"].value).contains("true");

///FIXME delete this line when DSLEditor will be corrected
talkToBase = false;
	
	Period = QString::fromStdString(params["sampleRate"].value).toInt()*1000;
	initialize();
	this->start();
}
TLaserData SpecificWorker::getLaserData()
{
	QMutexLocker mlocker(lDataMutex);
	return laserData;
}
TLaserData SpecificWorker::getLaserAndBStateData(RoboCompDifferentialRobot::TBaseState& state)
{
	QMutexLocker mlocker(lDataMutex);
	state = bState;
	return laserData;
}
LaserConfData SpecificWorker::getLaserConfData()
{
	return laserConf;
}
