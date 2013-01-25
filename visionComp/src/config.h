#ifndef CONFIG_H
#define CONFIG_H

// Comment this if you don't need the server code
#define SERVER

// Comment out this line if your application has a QtGui
// #define USE_QTGUI

#define PROGRAM_NAME    "Vision"
#define SERVER_FULL_NAME   "RoboComp::Vision"

#define PIRNIVELES 6
#define RADIUS 3

#define VISION_RADIUS_DEFAULT_VALUE  3
#define VISION_PYRLEVELS_DEFAULT_VALUE  6
#define MAX_CAMERAS 2
#define CHECK_PERIOD 3000


#define UMBRAL_HESS 15
#define UMBRAL_LAPLACIAN 5
#define UMBRAL_HARRIS 0.08
#define UMBRAL_HARRIS_MIN 0.05

#endif
