// -*-c++-*-

#include "observadormodelocoli.h"

#include <modelo-osg/modelo.h>
#include <rapid/rapid.h>

#include <iostream>
using std::cerr; 
using std::cout;
using std::endl;

namespace osgDTE {


  bool observadormodelocoli::updateFrom(Observado* obs)
  {
    no_hay_colision = true;
    par_colisionado = -1;
    std::vector <string>::iterator j;
      	
    modelo* m = dynamic_cast<modelo*>(obs);
    if (m)
      {
	for (unsigned int i = 0; i< m->_pares_colision.size(); i++) 
	{
	  for (j=_links_sospechosos.begin(); j!=_links_sospechosos.end(); j++)
	    {
	      if ( (m->_pares_colision[i].first->trf->getName()==*j)||(m->_pares_colision[i].second->trf->getName()==*j) )
		{
		  
		  if (haycolision(i,obs)) 
		    {
		      // registra los pares colisionados, como indices de los pares de colision
		      _pares_colisionados.push_back(i);

		      no_hay_colision = false;
		      par_colisionado = i;
		    }
		}
	    }
	}

      }
    else 
      {
	cerr << "Error en el observador de colisiones: " << __FILE__ << __LINE__ << std::endl;
      }
    return no_hay_colision;
  }

  void observadormodelocoli::incluirLink(string j)
  {
    _links_sospechosos.push_back(j);
  }


  void observadormodelocoli::quitarLink(string j)
  {
    std::vector <std::string>::iterator i;
    for (i=_links_sospechosos.begin(); i!=_links_sospechosos.end(); i++);
    {
      if (*i==j)
	_links_sospechosos.erase(i);
    }  
  }

  void observadormodelocoli::limpiarLinks()
  {
    _links_sospechosos.clear();
  }

  // FUNCIÓN PRIVADA
  // Comprueba si hay colisión dado un determinado par de colisión
  bool observadormodelocoli::haycolision(unsigned int i, Observado* obs)
  {
    double R1[3][3], R2[3][3], T1[3], T2[3];
    osg::Matrix global1, global2;

    modelo* m = dynamic_cast<modelo*>(obs);
    if (!m)
      {
	cerr<<"Error en Observadormodelocoli: Fallo al cargar el modelo en 'haycolision'"<<endl;
	return false;
      }

    // Obtiene las transformaciones de los joints
    global1 = computeLocalToWorld(m->_pares_colision[i].first->nodepath);
    global2 = computeLocalToWorld(m->_pares_colision[i].second->nodepath);
    
	
    R1[0][0] = global1(0,0);
    R1[0][1] = global1(1,0);
    R1[0][2] = global1(2,0);
    R1[1][0] = global1(0,1);
    R1[1][1] = global1(1,1);
    R1[1][2] = global1(2,1);
    R1[2][0] = global1(0,2);
    R1[2][1] = global1(1,2);
    R1[2][2] = global1(2,2);
	
    T1[0] = global1(3,0);
    T1[1] = global1(3,1);
    T1[2] = global1(3,2);
	
    R2[0][0] = global2(0,0);
    R2[0][1] = global2(1,0);
    R2[0][2] = global2(2,0);
    R2[1][0] = global2(0,1);
    R2[1][1] = global2(1,1);
    R2[1][2] = global2(2,1);
    R2[2][0] = global2(0,2);
    R2[2][1] = global2(1,2);
    R2[2][2] = global2(2,2);
	
    T2[0] = global2(3,0);
    T2[1] = global2(3,1);
    T2[2] = global2(3,2);
	
    // Calcula las colisiones teniendo en cuenta transf. de los joints 
    // y meshes (_modelo_colision)
    RAPID_Collide(R1,T1,m->_pares_colision[i].first->_modelo_colision,
		  R2,T2,m->_pares_colision[i].second->_modelo_colision, 
		  RAPID_FIRST_CONTACT);

    if (RAPID_num_contacts) return true;
    else return false;
  }

}
