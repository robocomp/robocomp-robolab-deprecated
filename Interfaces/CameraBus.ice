#include <CommonHead.ice>
#include <DifferentialRobot.ice>

/** \mainpage RoboComp Interfaces: CameraBus.ice
 *
 * \section intro_sec Introduction
* Interface for CameraBusComp.  
*                                                                   
* Basic interface for accessing a set of cameras connected to the same bus, p.e:
*  Possible sources of video                                                                     
*  FIREWIRE: IEEE1394 specification                                                          
*  V4L2: Video for Linux 2                                                                   
*  Prosilica: for using with Prosilica SDK                                                   
*  Point Grey: for using with propietary Point Grey's Flycapture SDK  
*
*  
*  PORT 10001 <br>   
*/

/** \namespace RoboCompCameraBus
  *@brief Name space of CameraBus interface.
  */

module RoboCompCameraBus
{
  /**@brief exception hardware*/
  exception HardwareFailedException { string what; };
  /**@brief exception moving image*/
  exception MovingImageException { string what; };
  /**@brief exception format not available*/
  exception FormatNotAvailable { string what; };
  
   /** \enum Mode
  *@brief enumerated type Mode with all types of known formats 
  */
  enum  Mode {YUV422, GRAY8, RGB888Packet, RGB888Planar };
  
   /** \enum captureMode
  *@brief enumerated type captureMode with all types of camera capture formats 
  */
  enum  captureMode { YUV411Mode, YUV422Mode, RGB8Mode, MONO16Mode, MONO8Mode, Format7Mode };
  
  
   /** \struct Format
  *@brief struct Format describing image parameters Mode, Width and Height.
  */
   struct Format
   {
     /// Mode of capure as specified in the enumerated Mode type.
     Mode modeImage;
     /// Width of image in pixels
     int width;
     /// Height of image in pixels
     int height;
	 /// Size of image in pixels
	 int size;
   };
   

   /** \struct CameraParams
  *@brief struct CameraParams describing name, bus identifier, intrinsic parameters and other configuration data.
  */
  struct CameraParams    //Configuration Params  
  {
    ///Name of the camera to be used when requesting images in getImage(string camera)
    string name; 
    /// Bus identifier
    byte busId;
    /// Focal in horizontal direction
    int focalX;
    /// Focal in vertical direction
    int focalY;
    /// Saturation
	int saturation;
	/// Inverted horizontally
	bool invertedH;
	/// Inverted vertically
	bool invertedV;
	/// 
	int lineFreq;
  };
   
   /** \struct CameraParamsList
  *@brief array of CameraParams holding a struct for each camera in the bus
  */
  sequence<CameraParams> CameraParamsList;
  
  /// List of operating system devices (usually one) connected to the physical bus (p.e. /dev/video0 ) 
  sequence<string> DeviceList;
  
   /** \struct BusParams
  *@brief Parameters specific of the underlying bus and shared by all the cameras.
  */
  struct BusParams
  {
	/// Devices
    DeviceList devices;    
    /// Number of cameras in the bus
    int numCameras;
    /// Frame rate for all the cameras
    int FPS;
    /// Width at which the driver is capturing the images
    int width;
    /// Height at which the driver is capturing the images
    int height;
    /// True if the component requests the odometry to the base of the robot before and after capturing an image, so it can tag the image with precise odometric information.
    bool talkToBase;
    /// True if the component requests the kinematic state of the robotic head before and after capturing and image so it can tag de image with precise kinematic information.
    bool talkToCommonHead;
	/// Capture mode 
	captureMode mode;	
  };
 
  sequence<byte> Frame;
  
  /** \struct Image
  *@brief Basic structure holding an image composed of a frame, a format and a timeStamp.
  */
  struct Image
  {
      Frame data;
      Format frmt;
      int timeStamp;
  };

  /** \struct ImageList
  *@brief Array of Images used for multicamera frame adquisition.
  */
  sequence<Image> ImageList;
  
    /** \struct CameraList
  *@brief List of camera names used to request selectively frames from the set of cameras
  */
  sequence<string> CameraList;

   /** \interface CameraBus
  *@brief interface CameraBus.
  */
  interface CameraBus
  {
    /// returns a vector of CameraParams containing one entry for each camera in the bus 
    idempotent CameraParamsList getAllCameraParams();

	/// return a struct containing the bus parameters
    idempotent BusParams getBusParams();

	/// returns an Image in the out parameter "image", from "camera" in the bus and with dimension and mode specified in "format"
    idempotent void getImage(string camera, Format frmt, out Image img);
	
	/// returns an Image in the out parameter "image", from "camera" in the bus and with dimension and mode specified in "format", also returns head and base positions
    idempotent void getImageAndStates(string camera, Format frmt,out Image img, out RoboCompCommonHead::THeadState hState,out RoboCompDifferentialRobot::TBaseState bState);
	
    /// returns a vector of Images in the out parameter "imageList", from cam)ras in "cameraList" and with dimension and mode specified in "format". If the parameter all is true, then one frame from all cameras is returned without checking the list "cameraList"
    idempotent void getSyncImages( CameraList cameras, Format frmt, bool all , out ImageList imglist );
	
	/// returns a vector of Images in the out parameter "imageList", from cameras in "cameraList" and with dimension and mode specified in "format". If the parameter all is true, then one frame from all cameras is returned without checking the list "cameraList", also returns head and base positions
    idempotent void getSyncImagesAndStates( CameraList cameras, Format frmt, bool all , out ImageList imglist, out RoboCompCommonHead::THeadState hState,out RoboCompDifferentialRobot::TBaseState bState );
  };
};
