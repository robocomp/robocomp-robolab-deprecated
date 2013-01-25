/* -*-c++-*- 
 * 
 * osgART - ARToolKit for OpenSceneGraph
 * Copyright (C) 2005-2008 Human Interface Technology Laboratory New Zealand
 * 
 * This file is part of osgART 2.0
 *
 * osgART 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * osgART 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with osgART 2.0.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OSGART_H
#define OSGART_H

#define VISIBLE 0xFFFFFFFF
#define INVISIBLE 0x0

/// QT includes
#include <QtCore>
#include <QtGui>
#include <QtOpenGL/QGLWidget>
#include <QTimer>
#include <QString>
/// RoboComp includes
#include <QMat/QMatAll>
#include "armarker.h"
#include "camara.h"
/// osgViewer includes
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/GraphicsWindow>
/// osg includes
#include <osg/Image>
#include <osg/Camera>
#include <osg/Notify>
#include <osg/NodeCallback>
#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>
#include <osg/Texture>
#include <osg/Texture2D>
#include <osg/TextureRectangle>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/LineSegment>
// #include <osg/io_utils>
/// osgUtil includes
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/IntersectVisitor>
#include <osgUtil/SceneView>
#include <osgUtil/PolytopeIntersector>
/// osgGA includes
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
/// osgDB incudes
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
/// osgART includes
#include <osgART/Marker>
#include <osgART/Tracker>
#include <osgART/Foundation>
#include <osgART/VideoLayer>
#include <osgART/PluginManager>
#include <osgART/VideoGeode>
#include <osgART/Utils>
#include <osgART/MarkerCallback>
#include <osgART/TransformFilterCallback>
// #include <osgART/GeometryUtils>
/// IPP includes
#include <ipp.h>
// #include <ipps.h>
// #include <ippdefs.h>
// #include <ippcc.h>
/// C++ standar libraries includes
#include <string>
#include <stdio.h>
#include <iostream>

using Qt::WindowFlags;

using namespace std;

class OsgArt : public QGLWidget, public osgViewer::Viewer
{
Q_OBJECT
  
  private:
	osg::ref_ptr<osg::Group> root;
	osg::ref_ptr<osg::Group> videoBackground;
	osg::ref_ptr<osg::Image> osgImage;
	osg::ref_ptr<osg::Camera> cam;
	
	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;
	
	osg::ref_ptr<osgART::Tracker> tracker;
	osg::ref_ptr<osgART::Calibration> calibration;
	
	/// Los marcadores marker guardan la información de las marcas ARToolKit que se han añadido al tracker.
	/// ---
	/// Markers keep the info of the ARToolKit markers added to the tracker.
	osg::ref_ptr<osgART::Marker> marker;
	osg::ref_ptr<osgART::Marker> marker2;
	
	/// Nodos con el sistema de referencia de la marca. Todos los objetos deben colgar de uno de ellos.
	/// ---
	/// Nodes containing the marker coordinates system. Every object in the scene must be below it in the node hierarchy.
	osg::ref_ptr<osg::MatrixTransform> arTransform;
	osg::ref_ptr<osg::MatrixTransform> arTransform2;
	
	/// markerSet guarda cada matriz de transformación con un nombre asociado, de forma que podemos buscar las MT por su nombre.
	/// ---
	/// markerSer containing the Matrix Transform with a associated name. We can searh the Matrix Transform by the associated name.
	QHash<QString,ARMarker> markerSet;
	
	osg::Matrix cameraMatrix;
	
	int imgSize, mThreshold,imgAlto, imgAncho;
	bool arriba,binarizar;
	Ipp8u *bufferImgResize;
	IppiSize bufferImgSize;
	Ipp8u *bufferImgResizeGray;
	Ipp8u *iv;
	
	
  protected:
	QTimer timer;
 
public:   
  
  uchar *data;
  
  OsgArt( QWidget* parent=0, const QGLWidget* shareWidget = 0, WindowFlags f = 0);
  void paintGL();
  void init();
  QVec updateART(std::vector<uchar> imgV);
  
  /// Methods defined inline
  void pose(osg::Vec3 pos){std::cout<<"x: "<<pos.x()<<" y: "<<pos.y()<<" z: "<<pos.z()<<std::endl;};
  osgViewer::GraphicsWindow* getGraphicsWindow(){return _gw.get();}
  bool visible(){return marker2->valid();};
  osg::MatrixTransform* getMatrixTransform1(){return arTransform;};
  osg::MatrixTransform* getMatrixTransform2(){return arTransform2;};
  osgART::Marker* getMarker1(){return marker;};
  osgART::Marker* getMarker2(){return marker2;};
  /// Añade una marca de AR bajo la cámara y a la lista de marcas
  /// ---
  /// Adds an AR marker as camera's child and to the marker list
  bool addMarker(string markerPath, string markerName, bool single, int size);

  /// Elimina una marca dado del grafo de OSG y de la lista de marcas
  /// ---
  /// Removes a given marker from the OSG graph and from the marker list
  bool removeMarker(string id);
  
  /// Obtiene el nodo de la matriz de transformación de una marca usando su id
  /// ---
  /// Gets the marker's transformation matrix node by its id
  osg::MatrixTransform* getMarkerNode(string id);
  
  QString getBaseMarker(osg::MatrixTransform* MT);
  
  /// Añade bajo el nodo arMT una esfera, siguiendo una jerarquía del tipo MatrixTransform->Geode->Drawable->Esfera
  /// ---
  /// Adds, as arMT's child, an sphere, using the node hierarchy MatrixTransform->Geode->Drawable->Sphere
  osg::MatrixTransform* addBall(osg::Vec3 position,int radius,osg::Vec4 color, bool transparent,osg::ref_ptr<osg::MatrixTransform> arMT);
  
  /// Añade bajo el nodo arMT un cono, siguiendo una jerarquía del tipo MatrixTransform->Geode->Drawable->Cono
  /// ---
  /// Adds, as arMT's child, a cone, using the node hierarchy MatrixTransform->Geode->Drawable->Cone
  osg::MatrixTransform* addCone(osg::Vec3 position,osg::Vec3 rotation,int coneRadius,int coneHeight,osg::Vec4 color, bool transparent,osg::ref_ptr<osg::MatrixTransform> arMT);
  
  /// Añade bajo el nodo arMT una caja, siguiendo una jerarquía del tipo MatrixTransform->Geode->Drawable->Caja
  /// ---
  /// Adds, as arMT's child, a box, using the node hierarchy MatrixTransform->Geode->Drawable->Box
  osg::MatrixTransform* addBox(osg::Vec3 position,osg::Vec3 rotation,osg::Vec3 dimensions,osg::Vec4 color, bool transparent,osg::ref_ptr<osg::MatrixTransform> arMT);
  
  /// Añade bajo el nodo arMT un cilindro, siguiendo una jerarquía del tipo MatrixTransform->Geode->Drawable->Cilindro
  /// ---
  /// Adds, as arMT's child, a cylinder, using the node hierarchy MatrixTransform->Geode->Drawable->Cylinder
  osg::MatrixTransform* addCylinder(osg::Vec3 position,osg::Vec3 rotation,float radius, float height,osg::Vec4 color, bool transparent,osg::ref_ptr<osg::MatrixTransform> arMT);
  
  /// Añade bajo el nodo arMT una cápsula, siguiendo una jerarquía del tipo MatrixTransform->Geode->Drawable->Cápsula
  /// ---
  /// Adds, as arMT's child, a capsule, using the node hierarchy MatrixTransform->Geode->Drawable->Capsule
  osg::MatrixTransform* addCapsule(osg::Vec3 position,osg::Vec3 rotation,float radius, float height,osg::Vec4 color, bool transparent,osg::ref_ptr<osg::MatrixTransform> arMT);
    
  /// Cambia el color en una jerarquía del tipo MatrixTransform
  /// ---
  /// Change the color in the node hierarchy MatrixTransform
  void setColorRGBA(osg::MatrixTransform* mt, osg::Vec4 color);
  
  /// Añade bajo el nodo arMT un texto, siguiendo una jerarquía del tipo MatrixTransform->Geode->Text
  /// ---
  /// Adds, as arMT's child, a text, using the node hierarcy MatrixTransform->Geode->Text
  osg::MatrixTransform* addText(osg::Vec3 position, osg::Vec4 color, float characterSize, std::string texto, osgText::Font* font, osg::ref_ptr<osg::MatrixTransform> arMT);
 
  /// Cambia la posicion del texto en una jerarquía del tipo MatrixTransform->Geode->Text
  /// ---
  /// Change the position of text in a hierarchy MatrixTransform-> Geode->Text
  void changePositionText(osg::MatrixTransform* arMT, osg::Vec3 position);
  
  /// Cambia el color del texto en una jerarquía del tipo MatrixTransform->Geode->Text
  /// ---
  /// Change the color of text in a hierarchy MatrixTransform-> Geode->Text
  void changeColorText(osg::MatrixTransform* arMT, osg::Vec4 color);
  
  /// Cambia el tamaño de letra del texto en una jerarquía del tipo MatrixTransform->Geode->Text
  /// ---
  /// Change the character size of text in a hierarchy MatrixTransform-> Geode->Text
  void changeCharacterSizeText(osg::MatrixTransform* arMT, float characterSize);
  
  /// Cambia la fuente del texto en una jerarquía del tipo MatrixTransform->Geode->Text
  /// ---
  /// Change the font of text in a hierarchy MatrixTransform-> Geode->Text
  void changeFontText(osg::MatrixTransform* arMT, osgText::Font* font);
  
  /// Cambia el texto del nodo arMT->Geode-Text
  /// ---
  /// Change the text in a hierarchy MatrixTransform-> Geode->Text
  void changeText(osg::MatrixTransform* arMT, osgText::String texto);
  
  /// Asigna una textura en 2D a un nodo arMT->Geode->Drawable->Box
  /// ---
  /// 
  void addTexture2D(osg::MatrixTransform* arMT, std::string imagenEntrada, osg::Vec4 valoresMaterial, float transparencia);

  /// Asigna una textura en 2D a un nodo arMT->Geode->Drawable->Box
  /// ---
  /// 
  void addTexture2D(osg::MatrixTransform* arMT, osg::Image *imagenEntrada, osg::Vec4 valoresMaterial, float transparencia);

  /// Asigna una textura en 3D a un nodo arMT->Geode->Drawable->Box
  /// ---
  /// 
  void load3DModel(osg::MatrixTransform* arMT, osg::Node* model, osg::Vec3d position, osg::Vec3d escala);
  
  /// Translada el objeto colgado de mt a una posición "position" realativa a su posición actual.
  /// ---
  /// Moves the object "mt" to "position". This movement is relative to the objet's current position.
  void move(osg::MatrixTransform* mt, osg::Vec3 position);
  
  /// Rota el objeto colgado de mt en los ejes XYZ, según los ángulos indicados en "angles".
  /// ---
  /// Rotates the object "mt" "angles" radians. This rotation is relative to the objet's current position.
  void rotate(osg::MatrixTransform* mt, osg::Vec3 angles);
  
  /// Rota el objeto colgado de mt en los ejes XYZ, según los ángulos indicados en "angles".
  /// ---
  /// Rotates the object "mt" "angles" radians. This rotation is relative to the objet's current position.
  void scale(osg::MatrixTransform* mt, osg::Vec3 axies);
  
  /// Pone visible el objeto dado en Node
  /// ---
  /// Set to visible the node
  void setVisible(osg::Node *node);
  
  /// Pone invisible el objeto dado en Nodo
  /// ---
  /// Set to invisible the node
  void setInvisible(osg::Node *node);
  
  /// Obtiene el objeto que aparece en las coordenadas (x,y) de la imagen renderizada.
  /// ---
  /// Gets the object appearint at the coordinates (x,y) of the rendered image. 
  osg::MatrixTransform* getObjectBy2DCoords(float x, float y);
    
  /// Mejora del picking
  /// ---
  /// Picking enhace
  osg::MatrixTransform* pick(float x,float y);
  
  /// Destructor por defecto
  /// ---
  /// Default destructor
 ~OsgArt();  


public slots:
	void resizeOsgArt ();  
	void binarize();  
	void closeOsgArt( );

signals:

};

#endif