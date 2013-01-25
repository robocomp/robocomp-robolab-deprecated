#ifndef ARMARKER_H
#define ARMARKER_H

#include <osg/MatrixTransform>
#include <osgART/Marker>
#include <string.h>

class ARMarker
{
private:
  osg::ref_ptr<osgART::Marker> marker;
  osg::ref_ptr<osg::MatrixTransform> markerMT;

  
public:
  
  ARMarker();
  ARMarker(osg::ref_ptr<osg::MatrixTransform> markerMT, osg::ref_ptr<osgART::Marker> marker);
  ARMarker(const ARMarker &ARM);
  
  void setMarker(osg::ref_ptr<osgART::Marker> marker);
  void setMT(osg::ref_ptr<osg::MatrixTransform> MT);
  
  osg::ref_ptr<osgART::Marker> getMarker();
  osg::ref_ptr<osg::MatrixTransform> getMT();
  
  bool operator==(ARMarker ARM) const;

  bool operator!=(ARMarker ARM) const;
  
  ~ARMarker();
};

#endif // ARMARKER_H
