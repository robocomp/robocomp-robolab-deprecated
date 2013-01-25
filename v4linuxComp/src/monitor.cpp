/*
 *    Copyright (C) 2010 by RoboLab - University of Extremadura
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
#include "monitor.h"
/**
* \brief Default constructor
*/
Monitor::Monitor(Worker *_worker,Ice::CommunicatorPtr _communicator)
{
	worker = _worker;
	this->communicator = _communicator;
	period = 100;
	state = RoboCompCommonBehavior::Starting;
}
/**
* \brief Default destructor
*/
Monitor::~Monitor()
{

}
/**
 * \brief Thread method
 */
void Monitor::run()
{
	initialize();
	forever
	{
		rDebug("monitor run");
		this->sleep(500);
	}
}

/**
* \brief Get component execution state
* @return State Component state
*/
RoboCompCommonBehavior::State Monitor::getState()
{
	return state;
}

/**
* \brief Get worker period
* @return int Worker period in ms
*/
int Monitor::getPeriod()
{
	return period;
}
/**
* \brief Change worker period
* @param per Period in ms
*/
void Monitor::setPeriod(int _period)
{
	worker->setPeriod(_period);
	period =_period;
}
/**
* \brief Kill component
*/
void Monitor::killYourSelf()
{
	rDebug("Killing myself");
	this->exit();
	worker->killYourSelf();
	emit kill();
	
}
/**
* \brief Get Component time awake
* @return int Time alive in seconds
*/
int Monitor::timeAwake()
{
	return initialTime.secsTo(QTime::currentTime());
}
/**
* \brief Return components parameters
* @return  AttrList Configuration parameters list
*/
RoboCompCommonBehavior::ParameterList Monitor::getParameterList() 
{ 
	return config_params;
}
/**
* \brief Change configurations parameters to worker
* @param l Configuration parameters list
*/
void Monitor::setParameterList(RoboCompCommonBehavior::ParameterList l) 
{ 
	rInfo("Changing configuration params");
	sendParamsToWorker(l);
}


/**
 * \brief Reads components parameters and checks set integrity before signaling the Worker thread to start running
 * There can be four (4) types of parameteres:
 *		(1) Ice parameters
 *		(2) Nexus (configuration) parameters	
 *		(3) Local component parameters read at start
 *		(4) Local parameters read from other running component
 *
 */
void Monitor::initialize()
{
	rInfo("Starting monitor ...");
	initialTime=QTime::currentTime();
	RoboCompCommonBehavior::ParameterList params;
	readConfig(params );
	if(!sendParamsToWorker(params))
	{
		rError("Error reading config parameters. Exiting");
		killYourSelf();
	}
	state = RoboCompCommonBehavior::Running;
}
bool Monitor::sendParamsToWorker(RoboCompCommonBehavior::ParameterList params)
{
	if(checkParams(params) == true)
	{
		//Set params to worker
		if(worker->setParams(params) == true)
			return true;
	}
	rError("Incorrect parameters");
	return false;
}
///Local Component parameters read at start
///Reading parameters from config file or passed in command line, with Ice machinery
///We need to supply a list of accepted values to each call
void Monitor::readConfig(RoboCompCommonBehavior::ParameterList &params )
{
	//Read params from config file
	RoboCompCommonBehavior::Parameter aux;
	aux.editable = false;
	aux.type = "string";
	configGetString( "V4linux.Device", aux.value,"/dev/video0");
	params["V4linux.Device"] = aux;
	configGetString("V4linux.captureMode",aux.value,"YUV422Mode");
	params["V4linux.captureMode"] = aux;	
	
	aux.type = "int";
	configGetString("V4linux.NumCameras",aux.value,"1");
	params["V4linux.NumCameras"] = aux;
	configGetString("V4linux.Width",aux.value,"320");
	params["V4linux.Width"] = aux;
	configGetString("V4linux.Height",aux.value,"240");
	params["V4linux.Height"] = aux;
	configGetString("V4linux.FPS",aux.value,"15");
	params["V4linux.FPS"] = aux;
	
	aux.type = "bool";
	configGetString("V4linux.TalkToBase",aux.value,"true");
	params["V4linux.TalkToBaseComp"] = aux;
	configGetString("V4linux.TalkToCommonHead",aux.value,"true");
	params["V4linux.TalkToCommonHead"] = aux;
	
	int num_cameras = QString::fromStdString(params["V4linux.NumCameras"].value).toInt();
	for(int i=0;i < num_cameras; i++)
	{
		std::string s= QString::number(i).toStdString();
		configGetString( "V4linux.Params_" + s, aux.value , "");
		params["V4linux.Params_" + s] = aux;
		QStringList list = QString::fromStdString(aux.value).split(",");
		if (list.size() != 8) qFatal("Error reading camera. Only %d parameters for camera %d, 7 needed.", list.size(), i);
		
		aux.type = "string";
		aux.value=list[0].toStdString();
		params["V4linux.Params_" + s +".name"]= aux;
		aux.value=list[1].toStdString();
		params["V4linux.Params_" + s +".busId"]= aux;
		aux.value=list[2].toStdString();
		aux.type = "bool";
		params["V4linux.Params_" + s +".invertedH"]= aux;
		aux.value=list[3].toStdString();
		params["V4linux.Params_" + s +".invertedV"]= aux;
		aux.value=list[4].toStdString();
		aux.type = "int";
		params["V4linux.Params_" + s +".focalX"]= aux;
		aux.value=list[5].toStdString();
		params["V4linux.Params_" + s +".focalY"]= aux;
		aux.value=list[6].toStdString();
		params["V4linux.Params_" + s +".saturation"]= aux;
		aux.value=list[7].toStdString();	
		params["V4linux.Params_" + s +".linefreq"]= aux;
	}
	    
}

//comprueba que los parametros sean correctos y los transforma a la estructura del worker
bool Monitor::checkParams(RoboCompCommonBehavior::ParameterList l)
{
	bool correct = true;
	//Check parameters
	//Example
// 	    if(l["DRobot.Handler"].value != "Robex" and l["DRobot.Handler"].value != "Gazebo" and l["DRobot.Handler"].value != "Player")
// 		    correct = false;
	if(l["V4linux.captureMode"].value != "YUV422Mode")
	{
		rError("Capture mode not supported: "+l["V4linux.captureMode"].value);
		correct = false;
	}
	if(QString::fromStdString(l["V4linux.NumCameras"].value).toInt() < 1)
	{
		rError("Error cameras can't be: "+l["V4linux.NumCameras"].value);
		correct = false;
	}
	//copy parameters
	if(correct)
		config_params = l;
	return correct;
}


//Ice Methods to read from file
bool Monitor::configGetString( const std::string name, std::string &value,  const std::string default_value, QStringList *list)
{
	value = communicator->getProperties()->getProperty( name );
	if ( value.length() == 0)
	{
		value = default_value;
		return true;
	}
	if(list != NULL)
	{
		if (list->contains(QString::fromStdString(value)) == false)
		{
			qFatal("Reading config file: %s is not a valid string", name.c_str());
			rError("Reading config file:"+name+" is not a valid string");
		}
	}
	std::cout << name << " " << value << std::endl;
	return true;
}
