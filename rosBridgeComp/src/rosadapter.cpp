#include "rosadapter.h"

RosAdapter::RosAdapter(SpecificWorker* _worker )
{
	worker = _worker;
	
	//Ros
	queue = ros::getGlobalCallbackQueue();
	joySub = rosNode.subscribe<sensor_msgs::Joy>("joy",10,&RosAdapter::joyCallback,this);
	pub = rosNode.advertise<std_msgs::String>("SpeakerState", 5);
	
	qDebug()<<"rosAdapter started";
}
RosAdapter::~RosAdapter(){
  qDebug()<<"kill rosAdapter";

}
//Message received from joystick topic publication
void RosAdapter::joyCallback(const sensor_msgs::Joy::ConstPtr& joy){
	qDebug()<<"adv"<<joy->axes[1]*500.<<"rot"<<joy->axes[0]*M_PI;
	//here we must use this information to connect with our components, for instance with speaker
	try
	{
		worker->speaker->setVolume(joy->axes[1]);
	}
	catch(Ice::Exception e)
	{
		qDebug()<<"error connecting to speaker, setting volume";
	}
}

void RosAdapter::run()
{
	rDebug("ros running");
	std_msgs::String str;
	std::string state = "running";
	forever
	{
		//Read state from speaker
		try
		{
			state = worker->speaker->getState();
		}
		catch(Ice::Exception e)
		{
			qDebug()<<"error connecting to speaker, reading state";
		}
		//publish speaker state into a ros topic
		str.data = state;
		pub.publish(str);
		queue->callAvailable(ros::WallDuration(0.1));
		usleep(100000);
	}
}
