
#ifndef ROSADAPTER_H
#define ROSADAPTER_H

// QT includes
#include <QtCore>

// User includes...
#include <config.h>

#include "specificworker.h"

//ros
#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/String.h>

class RosAdapter : public QThread
{
Q_OBJECT
public:

	RosAdapter( SpecificWorker *worker);
	~RosAdapter();

	void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
	void testC(const std_msgs::String::ConstPtr& joy);
	void run();


private:
	SpecificWorker *worker;
	
	//ros
	ros::NodeHandle rosNode;
	ros::CallbackQueue *queue;
	//publisher
	ros::Publisher  pub;	
	std_msgs::String state; //string used to published speaker state into a ros topic
	
	//suscriber
	ros::Subscriber joySub;
	ros::Subscriber test;
};

#endif