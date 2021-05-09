#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "map.h"
#include "funciones.h"
#include "menu.h"

void mostrarMenu (Map* mapaAlmacenamiento, Map* mapaPokedex) {
	bool salir = false;
	while (salir != true)
	{
		printf("\n**********************************************\n");
		printf("\n Bienvenido a menu Pokemon \n\n");
		printf("Seleccione una opcion: \n\n");
		printf("1. Atrapar Pokemon \n");
		printf("2. Evolucionar Pokemon \n");
		printf("3. Buscar Pokemon por tipo \n");
		printf("4. Buscar Pokemon por nombre \n");
		printf("5. Buscar Pokemon por nombre en pokedex \n");
		printf("6. Mostrar pokedex \n");
		printf("7. Mostrar los Pokemon ordenados por PC \n");
		printf("8. Liberar Pokemon \n");
    	printf("9. Mostrar Pokemon por region \n");
		printf("10.Extraer en archivo \n");
    	printf("0. Salir \n\n");
		printf("\n**********************************************\n");

		int opcion;
		scanf("%d", &opcion);
		
		switch (opcion) {
			case 1: atraparPokemon(mapaAlmacenamiento, mapaPokedex); break;
    		case 2: evolucionarPokemon(mapaAlmacenamiento, mapaPokedex); break;
			case 3: buscarPokemonPorTipo(mapaAlmacenamiento, mapaPokedex); break;
			case 4: buscarPokemonPorNombreAlmacenamiento(mapaAlmacenamiento); break;
			case 5: buscarPokemonPorNombrePokedex(mapaPokedex); break;
			case 6: mostrarPokedex(mapaPokedex); break;
			case 7: mostrarPokemonOrdenadosPorPc(mapaAlmacenamiento); break;
			case 8: liberarPokemon(mapaAlmacenamiento,mapaPokedex); break;
			case 9: mostrarPokemonPorRegion(mapaPokedex); break;
			case 10: exportarArchivo(mapaAlmacenamiento,mapaPokedex); break;
      		case 0 : salir = true; break;
			default : printf("Opcion no valida, intente nuevamente.\n");
		}
	}
}