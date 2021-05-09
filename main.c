#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "funciones.h"
#include "menu.h"

void importarPokemonsDesdeUnArchivo(Map* mapaAlmacenamiento, Map* mapaPokedex);

int main() {

	Map * mapaAlmacenamiento = createMap(is_equal_int);
	Map * mapaPokedex = createMap(is_equal_string);
	
  	importarPokemonsDesdeUnArchivo(mapaAlmacenamiento, mapaPokedex);

	mostrarMenu(mapaAlmacenamiento, mapaPokedex);

	return 0;

}
