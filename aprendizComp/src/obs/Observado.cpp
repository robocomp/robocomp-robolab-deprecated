#include <obs/Observado.h>
#include <obs/Observador.h>

void Observado::changed()
{

  // Si alguno de los observadores devuelve false a traves de su
  // metodo updateFrom, la cadena de actualizaciones se interrumpe.
  for (std::vector<Observador*>::iterator it = observadores.begin();
       it != observadores.end();
       it++)
    {
      if (*it) {
	if (!(*it)->updateFrom(this)) break;
      }
    }
}

