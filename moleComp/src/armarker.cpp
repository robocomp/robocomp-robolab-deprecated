#include "armarker.h"


ARMarker::ARMarker()
{
  this->markerMT=NULL;
  this->marker=NULL;
}

ARMarker::ARMarker(osg::ref_ptr< osg::MatrixTransform > markerMT, osg::ref_ptr< osgART::Marker > marker)
{
  this->markerMT = markerMT;
  this->marker = marker;
}

ARMarker::ARMarker(const ARMarker &ARM)
{
  this->markerMT=ARM.markerMT;
  this->marker=ARM.marker;
}


osg::ref_ptr< osgART::Marker > ARMarker::getMarker()
{
  return marker;
}

osg::ref_ptr< osg::MatrixTransform > ARMarker::getMT()
{
  return markerMT;
}

void ARMarker::setMarker(osg::ref_ptr< osgART::Marker > marker)
{
  this->marker = marker;
}

void ARMarker::setMT(osg::ref_ptr< osg::MatrixTransform > MT)
{
  this->markerMT = MT;
}


bool ARMarker::operator==(ARMarker ARM) const
{
  if(markerMT == ARM.getMT())
    return true;
  
  return false;
}

bool ARMarker::operator!=(ARMarker ARM) const
{
  if(this->markerMT != ARM.getMT())
    return true;
  
  return false;
}

ARMarker::~ARMarker()
{
  this->markerMT = NULL;
  this->marker = NULL;
}

