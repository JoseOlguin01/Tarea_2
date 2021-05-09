#ifndef funciones_h
#define funciones_h

typedef struct{
	int id;
	char nombre[30];
  	char tipos[50];
	int PC;
	int PS;
	char sexo[10];
}Almacenamiento;

typedef struct{
    char nombre[30];
    int existencia;
	char tipos[50];
	char evolucionPrevia[30];
	char evolucionPosterior[30];
	int numeroPokedex;
	char region[30];
}Pokedex;

int is_equal_int(void * key1, void * key2);

int lower_than_int(void * key1, void * key2);

int is_equal_string(void * key1, void * key2);

int lower_than_string(void * key1, void * key2);

char *get_csv_field (char * tmp, int k);

void importarPokemonsDesdeUnArchivo(Map* mapaAlmacenamiento,Map* mapaPokedex);

void exportarArchivo(Map* mapaAlmacenamiento,Map* mapaPokedex);

Almacenamiento* createAlmacenamiento (int id, char* nombre, int PC, int PS, char* sexo);

Pokedex* createPokedex (char* nombre, int existencia, char* tipos, char* evolucionPrevia, char* evolucionPosterior, int numeroPokedex, char* region);

void atraparPokemon(Map* mapaAlmacenamiento, Map* mapaPokedex);

void evolucionarPokemon(Map* mapaAlmacenamiento, Map* mapaPokedex);

void buscarPokemonPorTipo(Map* mapaAlmacenamiento, Map* mapaPokedex);

void buscarPokemonPorNombreAlmacenamiento(Map* mapaAlmacenamiento);

void buscarPokemonPorNombrePokedex(Map* mapaPokedex); 

int compararPokedex(const void *a, const void *b);

void mostrarPokedex(Map* mapaPokedex);

int compararPokemon(const void *a, const void *b);

void mostrarPokemonOrdenadosPorPc(Map* mapaAlmacenamiento);

void liberarPokemon(Map* mapaAlmacenamiento,Map* mapaPokedex);

void mostrarPokemonPorRegion(Map* mapaPokedex);



#endif