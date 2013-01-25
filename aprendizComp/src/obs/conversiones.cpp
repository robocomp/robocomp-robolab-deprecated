#include <obs/conversiones.h>

namespace osgDTE {


  void ModeloPersonaAHoap::AngulosBrazoIzquierdo(const modelobasicopersona& m, std::vector<double> & larm_joint)
  {
    //larm_joint.resize(4);

    // ANGULOS HOMBRO
    const osg::Matrix& homb = m.jBrazoIzq.trf->getMatrix();
	  
    // TODO: Mirar si estamos cerca de gimbal lock
	  
    // Si no lo estamos proceder con normalidad

    // OJO que las matrices de osg son:
    // 
    // |  R    0 |
    // |         |
    // |   T     |
    // |  t    1 |

    // LARM_JOINT[1] del hoap:
    larm_joint[0] = atan2(-homb(2,2),homb(2,1));
    ONDEBUG(std::cout << "\nLARM_JOINT[1] = " << larm_joint[0] * 180.0 / 3.14159265 << std::endl);

    // LARM_JOINT[2] del hoap
    larm_joint[1] = atan2(-homb(2,0),sqrt(homb(2,1)*homb(2,1) + homb(2,2)*homb(2,2)));
    ONDEBUG(std::cout << "LARM_JOINT[2] = " << larm_joint[1] * 180.0 / 3.14159265 << std::endl);
	  
    // LARM_JOINT[3] del hoap
    larm_joint[2] = atan2(homb(1,0),-homb(0,0));
    ONDEBUG(std::cout << "LARM_JOINT[3] = " << larm_joint[2] * 180.0 / 3.14159265 << std::endl); 
	  
	  
    // Si lo estamos llamar subrutina gimbal_lock
	  
	  	  
    // ANGULOS CODO
    const osg::Matrix& cod = m.jAntebrazoIzq.trf->getMatrix();
	  
    // LARM_JOINT[4] del hoap
    larm_joint[3] = atan2(cod(1,2),cod(1,1));
    ONDEBUG(std::cout << "LARM_JOINT[4] = " << larm_joint[3] * 180.0 / 3.14159265 << std::endl);
	  
  }

  void ModeloPersonaAHoap::AngulosBrazoDerecho(const modelobasicopersona& m, std::vector<double> & rarm_joint)
  {
    // ANGULOS HOMBRO
    const osg::Matrix& homb = m.jBrazoDer.trf->getMatrix();
	  
	  
    // RARM_JOINT[1] del hoap:
    rarm_joint[0] = atan2(-homb(2,2),homb(2,1));
    ONDEBUG(std::cout << "\nRARM_JOINT[1] = " << rarm_joint[0] * 180.0 / 3.14159265 << std::endl);
	  
    // RARM_JOINT[2] del hoap
    rarm_joint[1] = atan2(-homb(2,0),sqrt(homb(2,1)*homb(2,1) + homb(2,2)*homb(2,2)));
    ONDEBUG(std::cout << "RARM_JOINT[2] = " << rarm_joint[1] * 180.0 / 3.14159265 << std::endl);
	  
    // RARM_JOINT[3] del hoap
    rarm_joint[2] = atan2(homb(1,0),-homb(0,0));
    ONDEBUG(std::cout << "RARM_JOINT[3] = " << rarm_joint[2] * 180.0 / 3.14159265 << std::endl); 
	  
	  	  
    // TODO: Mirar si estamos cerca del gimbal lock
	  
    // ANGULOS CODO
    const osg::Matrix& cod = m.jAntebrazoDer.trf->getMatrix();

    // RARM_JOINT[4] del hoap
    rarm_joint[3] = atan2(cod(1,2),cod(1,1));
    ONDEBUG(std::cout << "RARM_JOINT[4] = " << rarm_joint[3] * 180.0 / 3.14159265 << std::endl);
	  
	  
  }
	
}
