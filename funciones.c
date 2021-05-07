#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "funciones.h"

/*
  función para comparar claves de tipo string
  retorna 1 si son iguales
*/
int is_equal_string(void * key1, void * key2) {
	if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

/*
  función para comparar claves de tipo string
  retorna 1 si son key1<key2
*/
int lower_than_string(void * key1, void * key2) {
	if(strcmp((char*)key1, (char*)key2) < 0) return 1;
    return 0;
}

/*
  función para comparar claves de tipo int
  retorna 1 si son iguales
*/
int is_equal_int(void * key1, void * key2) {
	if(*(int*)key1 == *(int*)key2) return 1;
    return 0;
}

/*
  función para comparar claves de tipo int
  retorna 1 si son key1<key2
*/
int lower_than_int(void * key1, void * key2) {
	if(*(int*)key1 < *(int*)key2) return 1;
    return 0;
}

/* -------------------------------------------------------------------------
 * Funcion que importa los Pokemons desde un archivo csv
 *------------------------------------------------------------------------*/
char *get_csv_field (char * tmp, int k) {
	int open_mark = 0;
	char* ret=(char*) malloc (100*sizeof(char));
	int ini_i=0, i=0;
	int j=0;
	while(tmp[i+1]!='\0'){

		if(tmp[i]== '\"'){
			open_mark = 1-open_mark;
			if(open_mark) ini_i = i+1;
			i++;
			continue;
		}

		if(open_mark || tmp[i]!= ','){
			if(k==j) ret[i-ini_i] = tmp[i];
			i++;
			continue;
		}

		if(tmp[i]== ','){
			if(k==j) {
				ret[i-ini_i] = 0;
				return ret;
			}
			j++; ini_i = i+1;
		}

		i++;
	}

	if(k==j) {
		ret[i-ini_i] = 0;
		return ret;
	}

	return NULL;
}

void importarPokemonsDesdeUnArchivo (Map* mapaAlmacenamiento, Map *mapaPokedex) {
	FILE *fp = fopen ("Pokemon.csv", "r");
	char linea[1024];

	while(fgets (linea, 1023, fp) != NULL) {
		char *aux;
		Almacenamiento* a = (Almacenamiento*) malloc (sizeof(Almacenamiento));
		Pokedex* p = (Pokedex*) malloc (sizeof(Pokedex));
		
		a->id = atoi(get_csv_field(linea,0));

		aux = get_csv_field(linea,1);
		strcpy(a->nombre,aux);
		strcpy(p->nombre,aux);
		
		aux = get_csv_field(linea,2);
		strcpy(p->tipos,aux);

		a->PC = atoi(get_csv_field(linea,3));
	
		a->PS = atoi(get_csv_field(linea,4));

		aux = get_csv_field(linea,5);
		strcpy(a->sexo,aux);
 
		aux = get_csv_field(linea,6);
		strcpy(p->evolucionPrevia,aux);	

		aux = get_csv_field(linea,7);
		strcpy(p->evolucionPosterior, aux);

		p->numeroPokedex = atoi(get_csv_field(linea, 8));

		aux = get_csv_field(linea,9);
		strcpy(p->region,aux);

		insertMap (mapaAlmacenamiento, &a->id, a);

		Pokedex *pokemon_buscado = searchMap(mapaPokedex, p->nombre);
		if (NULL == pokemon_buscado)	// No esta el pokemon
		{
			p->existencia = 1;
			insertMap (mapaPokedex, p->nombre, p);
		}
		else		// Si ya esta
		{
			pokemon_buscado->existencia++;
		}
	}
}

void extraerArchivo(Map* mapaAlmacenamiento,Map* mapaPokedex) {
	
	FILE *salida = fopen("Output.csv","wt");
    if (salida == NULL) exit(EXIT_FAILURE);

	Almacenamiento *pokemon = (Almacenamiento *) firstMap(mapaAlmacenamiento);
	Almacenamiento *vector_aux[101];
	int cantidad = 0;

	while (pokemon != NULL) {
		vector_aux[cantidad] = pokemon;
		cantidad++;

		Pokedex *p = searchMap(mapaPokedex, pokemon->nombre);

		fprintf(salida,"%i, %s, %s, %i, %i, %s, %s, %s, %i, %s\n", pokemon->id, pokemon->nombre, p->tipos, pokemon->PC, pokemon->PS, pokemon->sexo, p->evolucionPrevia,p->evolucionPosterior, p->numeroPokedex, p->region);

		pokemon = (Almacenamiento *) nextMap(mapaAlmacenamiento);
	}
	fclose(salida);
}

int compararPokemon(const void *a, const void *b)
{
	Almacenamiento **A = (Almacenamiento **) a;
	Almacenamiento **B = (Almacenamiento **) b;

	return (*B)->PC - (*A)->PC;
}

/*void buscarPokemonPorNombrePokedex(Map* mapaPokedex){
  
}*/

void mostrarPokemonOrdenadosPorPc(Map* mapaAlmacenamiento) {

	Almacenamiento *pokemon = (Almacenamiento *) firstMap(mapaAlmacenamiento);
	Almacenamiento *vector_aux[101];
	int cantidad = 0;

	while (pokemon != NULL) {
		vector_aux[cantidad] = pokemon;
		cantidad++;
		pokemon = (Almacenamiento *) nextMap(mapaAlmacenamiento);
	}
	
	qsort(vector_aux, cantidad, sizeof(Almacenamiento *), compararPokemon);

	for (int i = 0; i < cantidad; ++i) 
	{
		printf("%i %s %i %i %s\n", vector_aux[i]->id, vector_aux[i]->nombre, vector_aux[i]->PC, vector_aux[i]->PS, vector_aux[i]->sexo );
	}
}

int compararPokedex(const void *a, const void *b)
{
	Pokedex **A = (Pokedex **) a;
	Pokedex **B = (Pokedex **) b;

	return (*A)->numeroPokedex - (*B)->numeroPokedex;
}

void mostrarPokedex(Map* mapaPokedex) {

	Pokedex *pokemon = (Pokedex *) firstMap(mapaPokedex);
	Pokedex *vector_aux[101];
	int cantidad = 0;

	while (pokemon != NULL) {
		vector_aux[cantidad] = pokemon;
		cantidad++;
		pokemon = (Pokedex *) nextMap(mapaPokedex);
	}
	
	qsort(vector_aux, cantidad, sizeof(Pokedex *), compararPokedex);

	for (int i = 0; i < cantidad; ++i) 
	{
		printf("%s %i %s %s %s %i %s\n", vector_aux[i]->nombre, vector_aux[i]->existencia, vector_aux[i]->tipos, vector_aux[i]->evolucionPrevia, vector_aux[i]->evolucionPosterior,vector_aux[i]->numeroPokedex, vector_aux[i]->region);
	}
}

void mostrarPokemonPorRegion(Map* mapaPokedex) {
	char regionBuscada [30];
	int contador = 0;
	int regionEncontrada = 0;
	printf("Ingrese el nombre de la región que desea buscar: ");
	scanf("%s",regionBuscada);
	Pokedex *pokemon = (Pokedex *) firstMap(mapaPokedex);
	printf("\nRegión : %s\n\n",regionBuscada);
	while (pokemon != NULL) {
		if (strcmp(pokemon->region, regionBuscada)==0 ) {
			regionEncontrada = 1;
			printf("%s %i %s %s %s %i %s\n",pokemon->nombre, pokemon->existencia, pokemon->tipos, pokemon->evolucionPrevia, pokemon->evolucionPosterior, pokemon->numeroPokedex, pokemon->region);
			contador = contador + pokemon->existencia;
		}
		pokemon = (Pokedex *) nextMap(mapaPokedex);
	}
	if (regionEncontrada == 1)
		printf("\nPokemóns en almacenamiento de esa región : %i\n",contador);
	else
		printf("Región no encontrada");
}