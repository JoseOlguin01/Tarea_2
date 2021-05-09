#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "funciones.h"

/*
  función para comparar claves de tipo string, retorna 1 si son iguales
*/
int is_equal_string(void * key1, void * key2) {
	if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

/*
  función para comparar claves de tipo string, retorna 1 si son key1<key2
*/
int lower_than_string(void * key1, void * key2) {
	if(strcmp((char*)key1, (char*)key2) < 0) return 1;
    return 0;
}

/*
  función para comparar claves de tipo int, retorna 1 si son iguales
*/
int is_equal_int(void * key1, void * key2) {
	if(*(int*)key1 == *(int*)key2) return 1;
    return 0;
}

/*
  función para comparar claves de tipo int, retorna 1 si son key1<key2
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
    	strcpy(a->tipos,aux);
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

/* -------------------------------------------------------------------------
 * Funcion que exporta los Pokemons hacia un archivo csv
 *------------------------------------------------------------------------*/
void exportarArchivo(Map* mapaAlmacenamiento, Map* mapaPokedex) {
	
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

/* -------------------------------------------------------------------------
 * Funcion que crea almacenamiento para utilizar función atrapar pokemon 
 *------------------------------------------------------------------------*/
Almacenamiento* createAlmacenamiento (int id, char* nombre, int PC, int PS, char* sexo) {
    Almacenamiento* a = (Almacenamiento*) malloc (sizeof(Almacenamiento));
    a->id =  id;
	strcpy(a->nombre, nombre);
    a->PC = PC;
	a->PS = PS;
	strcpy(a->sexo, sexo);
    return a;
}

/* -------------------------------------------------------------------------
 * Funcion que crea pokedex para utilizar función atrapar pokemon 
 *------------------------------------------------------------------------*/
Pokedex* createPokedex (char* nombre, int existencia, char* tipos, char* evolucionPrevia, char* evolucionPosterior, int numeroPokedex, char* region) {
    Pokedex* p = (Pokedex*) malloc (sizeof(Pokedex));
	strcpy(p->nombre, nombre);
	p->existencia = existencia;
	strcpy(p->tipos, tipos);
	strcpy(p->evolucionPrevia, evolucionPrevia);
	strcpy(p->evolucionPosterior, evolucionPosterior);
	p->numeroPokedex = numeroPokedex;
	strcpy(p->region, region);
    return p;
}

/* -------------------------------------------------------------------------
 * Funcion que crea atrapa pokemon, es decir agrega un pokemon 
 *------------------------------------------------------------------------*/
void atraparPokemon(Map* mapaAlmacenamiento, Map* mapaPokedex) {
	Almacenamiento* A = (Almacenamiento*) malloc (sizeof(Almacenamiento));
	int elementosMapa = 0;
	A = firstMap(mapaAlmacenamiento);
    while (A) {
        elementosMapa++;
        A = nextMap(mapaAlmacenamiento);
    }
	/*se busca si la id ya esta en almacenamiento*/
	int auxId;
	Almacenamiento* aux;
	printf("Ingrese ID pokemon entre %d y 100 : ",elementosMapa);
	scanf("%d",&auxId);
	aux = searchMap(mapaAlmacenamiento, &auxId);
	/*si está se imprime que no se puede ingresar nuevo pokemon*/
	if (aux != NULL) {
		printf("ID no disponible, no se puede ingresar pokemon");
	}
	else
	/*si la ID no está en almacenamiento, permite que el usuario la ingrese*/
	{
		if (elementosMapa <= 100 ) {
			int auxPC, auxPS, auxExistencia, auxNumeroPokedex = 0;
			char auxNombre[30];
			char auxSexo[10];
			char auxTipos[50];
			char auxEvolucionPrevia[30];
			char auxEvolucionPosterior[30];
			char auxRegion[30];
			printf("Ingrese nombre pokemon : ");
			scanf("%s",auxNombre);
			printf("Ingrese PC pokemon : ");
			scanf("%d",&auxPC);
			printf("Ingrese PS pokemon : ");
			scanf("%d",&auxPS);
			printf("Ingrese sexo pokemon : ");
			scanf("%s",auxSexo);
			printf("Ingrese tipo pokemon : ");
			scanf("%s",auxTipos);
			printf("Ingrese evolucion previa pokemon : ");
			scanf("%s",auxEvolucionPrevia);
			printf("Ingrese evolucion posterior pokemon : ");
			scanf("%s",auxEvolucionPosterior);
			printf("Ingrese numero pokedex pokemon : ");
			scanf("%d",&auxNumeroPokedex);
			printf("Ingrese region pokemon : ");
			scanf("%s",auxRegion);

			Pokedex *pokemon_buscado = searchMap(mapaPokedex, auxNombre);
			/*si el pokomemon no existe se agrega a almacenamiento*/
			if (NULL == pokemon_buscado)	
			{
				Almacenamiento* a = createAlmacenamiento(auxId, auxNombre, auxPC, auxPS, auxSexo);
				insertMap(mapaAlmacenamiento, &a->id, a);
				Pokedex* p = createPokedex(auxNombre, 1, auxTipos, auxEvolucionPrevia, auxEvolucionPosterior, auxNumeroPokedex, auxRegion);
				insertMap(mapaPokedex, p->nombre, p);
				printf("\nPokemon atrapado exitosamente\n");

			}
			else
			/*si no está se suma 1 a existencia*/		
			{
				pokemon_buscado->existencia++;
			}
		}
		else
		{
			printf("No hay espacio suficiente en la pokedex");
		}
	}	
}

/* -------------------------------------------------------------------------
 * Funcion que evoluciona un pokemon, se busca y se actualizan sus datos
 *------------------------------------------------------------------------*/
void evolucionarPokemon(Map* mapaAlmacenamiento, Map* mapaPokedex) {
	int pokemonId;
	Almacenamiento *a;
	Pokedex *p;

  	printf("\nIngrese la ID del pokemon que desea evolucionar: \n");
  	scanf("%d",&pokemonId);

	/*se busca en mapaAlmacenamiento elemento en almacenamientoy en pokedex*/
 	a = searchMap(mapaAlmacenamiento, &pokemonId);
	p = searchMap(mapaPokedex, a->nombre);

	if (strcmp(a->nombre, p->evolucionPosterior) == 0) {
		printf("Pokemon ya se encuentra evolucionado");
		a->PC = ((a->PC)+((a->PC)*0.5));
		a->PS = ((a->PS)+((a->PS)*0.25));
	}
	if (strcmp("No tiene", p->evolucionPosterior) == 0) {
		printf("Pokemon no tiene evolucion");
		a->PC = ((a->PC)+((a->PC)*0.5));
		a->PS = ((a->PS)+((a->PS)*0.25));
	}
	if ((strcmp(a->nombre, p->evolucionPosterior) != 0) || (strcmp("No tiene", p->evolucionPosterior) != 0)) {
		strcpy(a->nombre,p->evolucionPosterior);
		strcpy(p->nombre,p->evolucionPosterior);
		a->PC = ((a->PC)+((a->PC)*0.5));
		a->PS = ((a->PS)+((a->PS)*0.25));
	}

	printf("\nDatos del pokemon evolucionado :\n");
	printf("%d, %s, %s, %d, %d, %s, %s, %s, %d, %s", a->id, a->nombre, p->tipos, a->PC, a->PS, a->sexo, p->evolucionPrevia, p->evolucionPosterior, p->numeroPokedex, p->region);
}

/* -------------------------------------------------------------------------
 * Función que busca por tipos los Pokémon del almacenamiento y muestra sus
   Respectivas informaciónes de combate
 *------------------------------------------------------------------------*/
void buscarPokemonPorTipo(Map* mapaAlmacenamiento, Map* mapaPokedex) {
  	char tipoBuscado [30];
  	int encontrado = 0;
	printf("\nIngrese el nombre del tipo de Pokemon que desea buscar: \n");
	scanf("%s",tipoBuscado);
	Almacenamiento *pokemon = (Almacenamiento *) firstMap(mapaAlmacenamiento);
	printf("\n                             Tipo : %s\n\n",tipoBuscado);
	printf(" ____________________________________________________________________\n");
	printf("|    Nombre     |             Tipo             |    PC    |    PS    |\n");
	printf(" ____________________________________________________________________\n");

	while (pokemon != NULL) {
		if (strstr(pokemon->tipos, tipoBuscado) != NULL ) {
			printf("|%15s|%30s|%10i|%10i|\n",pokemon->nombre, pokemon->tipos, pokemon->PC, pokemon->PS);
      		encontrado = 1;
		}
		pokemon = (Almacenamiento *) nextMap(mapaAlmacenamiento);
	}
	printf(" ____________________________________________________________________\n");
  	if (encontrado==0) {
    	printf("El Pokemon que desea no es parte de su almacenamiento o no existe\n");
  	}
}

/* -------------------------------------------------------------------------
 * Función que busca por nombre los Pokémon del almacenamiento y muestra sus
   Respectivas informaciónes de combate
 *------------------------------------------------------------------------*/
void buscarPokemonPorNombreAlmacenamiento(Map* mapaAlmacenamiento) {
  	char nombreBuscado [30];
  	int encontrado = 0;
	printf("Ingrese el nombre del o los Pokemon que desea buscar: ");
	scanf("%s",nombreBuscado);
	Almacenamiento *pokemon = (Almacenamiento *) firstMap(mapaAlmacenamiento);
	printf("\n                           Nombre : %s\n\n",nombreBuscado);
	printf(" _____________________________________\n");
	printf("|    Nombre     |    PC    |    PS    |\n");
	printf(" _____________________________________\n");
	while (pokemon != NULL) {
		if (strcmp(pokemon->nombre, nombreBuscado)==0 ) {
			printf("|%15s|%10i|%10i|\n",pokemon->nombre, pokemon->PC, pokemon->PS);
      		encontrado=1;
		}
		pokemon = (Almacenamiento *) nextMap(mapaAlmacenamiento);
	}
	printf(" _____________________________________\n");
  	if (encontrado == 0) {
    	printf("El Pokemon ya no es parte de su almacenamiento o no existe\n");
  	}
}

/* -------------------------------------------------------------------------
 * Función que busca por nombre los Pokémon de la Pokédex
 *------------------------------------------------------------------------*/
void buscarPokemonPorNombrePokedex(Map* mapaPokedex) {
 	char pokemonBuscado [30];
	Pokedex* aux ;
  	printf("Ingrese el nombre del Pokemon que desea buscar: ");
	scanf("%s",pokemonBuscado);
  	aux=searchMap(mapaPokedex,pokemonBuscado);
  	if(aux!=NULL){
		printf("\n                           Nombre : %s\n",pokemonBuscado);
		printf(" _______________________________________________________________________________________________________________\n");
		printf("|    Nombre     |existencia|         tipos              |     Ev. Previa | Ev. Posterior |n° Pokedex|  region   |\n");
		printf(" _______________________________________________________________________________________________________________\n");
    	printf("|%15s|%10i|%28s|%16s|%15s|%10i|%11s|\n",aux->nombre, aux->existencia, aux->tipos, aux->evolucionPrevia, aux->evolucionPosterior, aux->numeroPokedex, aux->region);
		printf(" ________________________________________________________________________________________________________________\n");
  	}
	else
	{
    	printf("El Pokemon buscado no existe");
  	}
}

/* -------------------------------------------------------------------------
 * Función de comparación de Numeros en la Pokédex
 *------------------------------------------------------------------------*/
int compararPokedex(const void *a, const void *b)
{
	Pokedex **A = (Pokedex **) a;
	Pokedex **B = (Pokedex **) b;

	return (*A)->numeroPokedex - (*B)->numeroPokedex;
}
/* -------------------------------------------------------------------------
 * Función que muestra los Pokémon de la Pokédex
 *------------------------------------------------------------------------*/
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

	printf("\n                           Pokedex : \n");
	printf(" _______________________________________________________________________________________________________________\n");
	printf("|    Nombre     |existencia|         tipos              |     Ev. Previa | Ev. Posterior |n° Pokedex|  region   |\n");
	printf(" _______________________________________________________________________________________________________________\n");
	for (int i = 0; i < cantidad; ++i) 
	{
		printf("|%15s|%10i|%28s|%16s|%15s|%10i|%11s|\n", vector_aux[i]->nombre, vector_aux[i]->existencia, vector_aux[i]->tipos, vector_aux[i]->evolucionPrevia, vector_aux[i]->evolucionPosterior,vector_aux[i]->numeroPokedex, vector_aux[i]->region);
	}
	printf(" _______________________________________________________________________________________________________________\n");
	
}

/* -------------------------------------------------------------------------
 * Función de comparación de PC
 *------------------------------------------------------------------------*/
int compararPokemon(const void *a, const void *b)
{
	Almacenamiento **A = (Almacenamiento **) a;
	Almacenamiento **B = (Almacenamiento **) b;

	return (*B)->PC - (*A)->PC;
}

/* -------------------------------------------------------------------------
 * Funcion que muestra los Pokémon del Almacenamiento ordenados por PC de 
   mayor a menor
 *------------------------------------------------------------------------*/
void mostrarPokemonOrdenadosPorPc(Map* mapaAlmacenamiento) {

	Almacenamiento *A = (Almacenamiento *) firstMap(mapaAlmacenamiento);
	Almacenamiento *vector_aux[101];
	int elementosMapa = 0;

	while (A != NULL) {
		vector_aux[elementosMapa] = A;
		elementosMapa++;
		A = (Almacenamiento *) nextMap(mapaAlmacenamiento);
	}
	printf(" _____________________________________________________________\n");
	printf("|    ID     |    Nombre    |    PC    |    PS    |    Sexo    |\n");
	printf(" _____________________________________________________________\n");
	qsort(vector_aux, elementosMapa, sizeof(Almacenamiento *), compararPokemon);

	for (int i = 0; i < elementosMapa; ++i) 
	{
		printf("|%11i|%14s|%10i|%10i|%12s|\n", vector_aux[i]->id, vector_aux[i]->nombre, vector_aux[i]->PC, vector_aux[i]->PS, vector_aux[i]->sexo );
	}
}

/* -------------------------------------------------------------------------
 * Funcion que libera un Pokémon del Almacenamiento y disminuye su
   existencia en la Pokédex
 *------------------------------------------------------------------------*/
void liberarPokemon(Map* mapaAlmacenamiento,Map* mapaPokedex){
  	int pokemonId;
	Almacenamiento* aux ;
  	Pokedex* aux2;
  	printf("Ingrese el ID del Pokemon que desea liberar: ");
	scanf("%d",&pokemonId);
	aux=eraseMap(mapaAlmacenamiento,&pokemonId);
	if (aux != NULL) {
		aux2 = searchMap(mapaPokedex,aux->nombre);
		if (aux2->existencia>0) {
			aux2->existencia--;
			printf("\nPokemon con ID %d eliminado exitosamente.\n",pokemonId);
		}
	}else{
    printf("El ID de Pokemon no existe\n");
  }
}

/* -------------------------------------------------------------------------
 * Funcion que muestra los Pokémon de la pokédex por Región y su cantidad
   en el almacenamiento
 *------------------------------------------------------------------------*/
void mostrarPokemonPorRegion(Map* mapaPokedex) {
	char regionBuscada [30];
	int contador = 0;
	int regionEncontrada = 0;
	printf("Ingrese el nombre de la region que desea buscar: ");
	scanf("%s",regionBuscada);
	Pokedex *pokemon = (Pokedex *) firstMap(mapaPokedex);
	printf("\nRegión : %s\n\n",regionBuscada);
  		printf(" ____________________________________________________________________________________________________________\n");
		printf("|    Nombre     |existencia|            tipos            | Ev. Previa | Ev. Posterior |n° Pokedex|  region   |\n");
		printf(" ____________________________________________________________________________________________________________\n");
	while (pokemon != NULL) {
		if (strcmp(pokemon->region, regionBuscada)==0 ) {
			regionEncontrada = 1;
			printf("|%15s|%10i|%29s|%12s|%15s|%10i|%11s|\n",pokemon->nombre, pokemon->existencia, pokemon->tipos, pokemon->evolucionPrevia, pokemon->evolucionPosterior, pokemon->numeroPokedex, pokemon->region);
			contador = contador + pokemon->existencia;
		}
		pokemon = (Pokedex *) nextMap(mapaPokedex);
	}
  	printf(" ____________________________________________________________________________________________________________\n");
	if (regionEncontrada == 1){
		printf("\nPokemon en almacenamiento de esa region : %i\n",contador);
  }else{
		printf("Region no encontrada");
  }
}

