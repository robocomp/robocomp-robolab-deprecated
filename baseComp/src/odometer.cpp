#include "odometer.h"

Odometer::Odometer(BaseHandler *baseHandler)
 : QObject() , _baseHandler(baseHandler)
{
}


Odometer::~Odometer()
{
}

/// Integrates velocity to update position of the base
/// Center Axis Advance = (VI + VD)/2  Center Axis Rotation (VI - VD)/2 
void Odometer::compute()
{
}

void Odometer::resetOdometer()
{
}
