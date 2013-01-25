//////////////////////////////////////////////////////
// Robolab component model prototype
//
//
// Robolab 2006
//////////////////////////////////////////////////////

#include <Ice/Ice.h>
#include <Ice/Application.h>
#include <IceUtil/IceUtil.h>

#include <QtCore/QtCore>
#include <QtCore/QCoreApplication>

// Interface implementation
#include "baseI.h"

// Includes for remote proxy example
// #include <Remote.h>

// Defaults
#define DEFAULT_DEVICE              "/dev/ttyUSB0"
// Properties names
#define PROPERTY_NAME_DEVICE    "Device"
#define PROPERTY_NAME_GEAR    "Gear"

using namespace std;

class BaseComp : public Ice::Application
{
private:
	// User private data here
	string device;

	void initialize();

	// Helper functions for read properties from config file
	bool configGetString( const std::string name, std::string &value, const std::string default_value = "" );
	bool configGetInt( const std::string name, int &value, const int default_value  = 0 );

public:
	virtual int run(int, char*[]);
};

bool BaseComp::configGetString( const std::string name, std::string &value, const std::string default_value )
{
	value = communicator()->getProperties()->getProperty( name );
	if ( value.length() == 0)
	{
		value = default_value;
		return false;
	}

	return true;
}

bool BaseComp::configGetInt( const std::string name, int &value, const int default_value )
{
	string tmp;

	tmp = communicator()->getProperties()->getProperty( name );
	if ( (tmp.length() == 0) || (std::atoi( tmp.c_str() ) == -1) )
	{
		value = default_value;
		return false;
	}

	value = std::atoi( tmp.c_str() );
	return true;
}

void BaseComp::initialize()
{
	configGetString(PROPERTY_NAME_DEVICE, device, DEFAULT_DEVICE);
	std::cout << "[0] baseComp: Device: " << device << "\n";
}

int main(int argc, char* argv[])
{
	bool hasConfig = false;
	string arg;
	BaseComp app;

	// Search in argument list for --Ice.Config= argument
	for (int i = 1; i < argc; ++i)
	{
		arg = argv[i];
		if ( arg.find ( "--Ice.Config=", 0 ) != string::npos )
			hasConfig = true;
	}

	if (hasConfig) return app.main( argc, argv );
	else return app.main(argc, argv, "config"); // "config" is the default config file name

}

int BaseComp::run(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);

	int status = EXIT_SUCCESS;
	initialize();

	BaseHandler baseHandler(device.c_str());

	try
	{
		// Server adapter creation and publication
		Ice::ObjectAdapterPtr base_adapter = communicator()->createObjectAdapter("BaseComp");
		BaseI *baseI = new BaseI( &baseHandler );
		base_adapter->add(baseI, communicator()->stringToIdentity("base"));
		base_adapter->activate();
	
		cout << "Robolab::Base started and running" << endl;
		a.exec();
			
		status = EXIT_SUCCESS;
	}
	catch(const Ice::Exception& ex)
	{
		status = EXIT_FAILURE;
	}

	return status;
}
