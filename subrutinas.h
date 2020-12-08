#ifndef SUBRUTINAS_H
#define SUBRUTINAS_H

//LIBRERIAS***********************************************************************


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

//DEFINICIONES********************************************************************


#define LONGITUD_LISTA      10000
#define LONGITUD_PALABRA    20
#define ESPANOL     1
#define INGLES      0
#define DICOTOMICA  1
#define SECUENCIAL  0


//ESTRUCTURA DE DATOS*************************************************************


/*********************************************************************************
 * Es la estructura que permite crear la lista y tiene el siguiente formato      *
 *                                                                               *
 *  [palabra en español] [palabra en ingles] [cantidad de busquedas]             *
 *                                                                               *
 *********************************************************************************/
typedef struct diccionario{
    char espanol[LONGITUD_PALABRA];
    char ingles[LONGITUD_PALABRA];
    int cant_busquedas;
}diccionario;

diccionario listado[LONGITUD_LISTA];

//FUNCIONES/SUBRUTINAS************************************************************


/*********************************************************************************
 * Funcion INICIAR(listado)                                                      *
 *                                                                               *
 *  Abre y lee el archivo y lo guarda en una estructura de tipo diccionario.     *
 *  La variable puntero guarda la posicion de la ultima palabra, para            *
 *  poder tener seguimiento en las demas funciones.                              *
 *                                                                               *
 *********************************************************************************/
int INICIAR(diccionario listado[]){
    setlocale(LC_ALL, "");
    int puntero = 0;
    FILE *diccionario;
    diccionario = fopen("espaingl.dat","r");

    while (!feof(diccionario)){
        fscanf(diccionario, "%s\t%s\t%d", listado[puntero].espanol, listado[puntero].ingles, &listado[puntero].cant_busquedas);
        puntero++;
    }

    fclose(diccionario);
    return puntero;
};


/*********************************************************************************
 * Subrutina FINALIZAR(listado, puntero)                                         *
 *                                                                               *
 *  Abre el archivo para guardar el listado ya ordenado. Luego cierra el         *
 *  archivo.                                                                     *
 *                                                                               *
 *********************************************************************************/
void FINALIZAR(diccionario listado[], int puntero){
    FILE *diccionario;
    diccionario = fopen("espaingl.dat","w");
    for (int i = 0; i<(puntero-1); i++){
        fprintf(diccionario, "%s %s %d\n", listado[i].espanol, listado[i].ingles, listado[i].cant_busquedas);
    }
    fclose(diccionario);
};


/*********************************************************************************
 * Funcion BUSCAR(palabra, listado, puntero, idioma)                             *
 *                                                                               *
 *  Busca la palabra en funcion del idioma de esta. De manera secuencial si      *
 *  es ingles, y dicotomica si es en español. Devuelve la posicion de la         *
 *  palabra buscada.                                                             *
 *                                                                               *
 *********************************************************************************/
int BUSCAR(char palabra[], diccionario listado[], int puntero, short idioma){
    int posicion;
    int i = 0;
    int min = 0;
    int max = puntero;

    char busqueda[LONGITUD_PALABRA];
    strcpy(busqueda, palabra);

    for (int i = 0; i < strlen(busqueda); i++){
        if (isspace(busqueda[i])){
            busqueda[i] = '_';
        }
    }

    if (idioma == INGLES){
        while(i<puntero && strcmp(busqueda, listado[i].ingles) != 0){
            i++;
        }
        if (strcmp(busqueda, listado[i].ingles)==0){
            posicion = i;
        }else{
            posicion = -1;
        }
    }else{
        i = (min+max)/2;
        while(min<max && strcmp(busqueda, listado[i].espanol) != 0){
            if(strcmp(busqueda, listado[i].espanol)<0){
                max = i-1;
            }else{
                min = i+1;
            }
            i = (min+max)/2;
        }
        if (strcmp(busqueda, listado[i].espanol)==0){
            posicion = i;
        }else{
            posicion = -1;
        }
    }
    return posicion;
};


/*********************************************************************************
 * Funcion ORDENAR_ALFABETICAMENTE(listado, puntero)                             *
 *                                                                               *
 *  Ordena la lista, la usamos cada vez que agregamos palabras.                  *
 *                                                                               *
 *********************************************************************************/
void ORDENAR_ALFABETICAMENTE(diccionario listado[], int puntero){
    int  j;
    short flag;
    diccionario aux;
    for (int i = 1; i<puntero; i++){
        aux = listado[i];
        j = i-1;
        flag = 1;
        while (flag && j>=0){

            if (strcmp(aux.espanol, listado[j].espanol)<0){
                listado[j+1] = listado[j];
                j--;
            }else{
                flag = 0;
            }
        }
        listado[j+1] = aux;
    }
};


/*********************************************************************************
 * Funcion MAS_BUSCADAS(listado, puntero)                                        *
 *                                                                               *
 *  Ordena la lista con respecto a la cantidad de veces que fueron buscadas      *
 *  de mayor a menor, las informa y vuevle a ordenar la lista de manera          *
 *  alfabetica.                                                                  *
 *                                                                               *
 *********************************************************************************/
void MAS_BUSCADAS(diccionario listado[], int puntero){
    int  j;
    short flag;
    diccionario aux;
    for (int i = 1; i<puntero; i++){
        aux = listado[i];
        j = i-1;
        flag = 1;
        while (flag && j>=0){
            if (aux.cant_busquedas>listado[j].cant_busquedas){
                listado[j+1] = listado[j];
                j--;
            }else{
                flag = 0;
            }
        }
        listado[j+1] = aux;
    }
    printf("\tMAS BUSCADOS\n");
    for (int i = 0; i<10; i++){
        printf(" %d - %s / %s con %d busquedas\n", i+1, listado[i].espanol, listado[i].ingles, listado[i].cant_busquedas);
    }
    ORDENAR_ALFABETICAMENTE(listado, puntero);
};


/*********************************************************************************
 * Funcion AGREGAR(palabra, listado, puntero, idioma)                            *
 *                                                                               *
 *  Funcion que permite agregar palabras en funcion al idioma de estas. En       *
 *  caso de que la lista se complete,este informa la situacion y no la agrega    *
 *                                                                               *
 *********************************************************************************/
int AGREGAR(char palabra[], diccionario listado[], int puntero, short idioma){
    char traduccion[LONGITUD_PALABRA];
    char seleccion = 'N';
    if(puntero<LONGITUD_LISTA && BUSCAR(palabra, listado, puntero, idioma)<0){
        while (seleccion == 'N'){
            if (idioma == ESPANOL){
                printf("Ingresar la traduccion al ingles\n ");
            }else{
                printf("Ingresar la traduccion al español\n ");
            }
            fflush(stdin);
            gets(traduccion);

            printf("¿La traduccion de %s es %s? (Y/N)\n ", palabra, traduccion);
            scanf(" %c", &seleccion);
            seleccion = toupper(seleccion);

            while (seleccion != 'Y' && seleccion != 'N'){
                printf("Ingrese una opcion correcta\n ");
                scanf(" %c", &seleccion);
                seleccion = toupper(seleccion);
            }

            for (int i = 0; i < strlen(traduccion); i++){
                if (isspace(traduccion[i])){
                    traduccion[i] = '_';
                }
            }

            if (seleccion == 'Y'){
                if (idioma == ESPANOL){
                    strcpy(listado[puntero].espanol, palabra);
                    strcpy(listado[puntero].ingles, traduccion);
                    listado[puntero].cant_busquedas = 0;
                }else{
                    strcpy(listado[puntero].ingles, palabra);
                    strcpy(listado[puntero].espanol, traduccion);
                    listado[puntero].cant_busquedas = 0;
                }
                printf("Palabra agregada con exito!\n ");
                puntero++;
            }
        }
    }else if(puntero>=LONGITUD_LISTA){
        printf("Se alcanzo el tope del diccionario, no se pueden agregar mas palabras!\n ");
    }else if(BUSCAR(palabra, listado, puntero, idioma)>=0){
        printf("La palabra buscada ya existe!\n ");
    }
    return puntero;
};


/*********************************************************************************
 * Funcion TRADUCIR(listado, puntero, idioma)                                    *
 *                                                                               *
 *  Dada una palabra y el idioma de esta, busca su posicion dentro de la         *
 *  lista y luego muestra su traduccion, en caso de que no se encuentre          *
 *  y el usuario quiera añadirla, llama a la funcion AGREGAR. Devuelve la        *
 *  posicion de la ultima palabra en la lista.                                   *
 *                                                                               *
 *********************************************************************************/
int TRADUCIR(diccionario listado[], int puntero, short idioma){
    int i = 0;
    int ant;
    char seleccion;
    char palabra[LONGITUD_PALABRA];

    printf(" Ingrese la palabra a traducir\n ");

    fflush(stdin);  //Limpia la entrada de datos, sierve para que funcione bien el gets
    gets(palabra);  //Permite escribir con espacios

    i = BUSCAR(palabra, listado, puntero, idioma);

    if (i>=0){
        listado[i].cant_busquedas = listado[i].cant_busquedas + 1;
        printf(" La traduccion de %c%s%c es ", '"', palabra,'"');
        if(idioma == ESPANOL){
            printf("%c%s%c\n\n ", '"', listado[i].ingles, '"');
        }else{
            printf("%c%s%c\n\n ", '"', listado[i].espanol, '"');
        }
    }else{
        printf(" Palabra no encontrada, ¿Desea agregarla al diccionario? (Y/N)\n ");
        scanf(" %c", &seleccion);
        seleccion = toupper(seleccion);
        while (seleccion != 'Y' && seleccion != 'N'){
            printf("Seleccione una opcion correcta!\n ");
            scanf(" %c", &seleccion);
            seleccion = toupper(seleccion);
        }
        if (seleccion == 'Y'){
            puntero = AGREGAR(palabra, listado, puntero, idioma);
            ORDENAR_ALFABETICAMENTE(listado, puntero);
        }
    }
    return puntero;
};


/*********************************************************************************
 * Funcion SUBMENU(listado, puntero, idioma)                                     *
 *                                                                               *
 *  Genera el submenu para un determinado idioma, con las opciones               *
 *  traducir, agregar al diccionario, mostrar las palabras msa buscadas          *
 *  y volver al menu anterior.                                                   *
 *                                                                               *
 *********************************************************************************/
short SUBMENU(diccionario listado[], int puntero, short idioma){
    char seleccion;
    char seleccion2 = 'Y';
    char palabra[LONGITUD_PALABRA];
    short retorno;
    printf("-----------------------------------------------------------------------\n");
    if(idioma == INGLES){
        printf("\t  INGLES A ESPAÑOL\n");
    }else{
        printf("\t  ESPAÑOL A INGLES\n");
    }
    printf(" T - Traducir\n A - Agregar palabra \n M - Mas buscadas \n V - Volver\n ");
    scanf(" %c", &seleccion);
    seleccion = toupper(seleccion);
    while(seleccion != 'T' && seleccion != 'A' && seleccion != 'M' && seleccion != 'V'){
        printf("Ingrese una opcion correcta!\n ");
        scanf(" %c", &seleccion);
        seleccion = toupper(seleccion);
    }

    switch (seleccion){
    case 'T':
        puntero = TRADUCIR(listado, puntero, idioma);
        retorno = 1;
        break;

    case 'A':
        while (seleccion2 == 'Y'){
            printf("Ingrese la palabra a agregar\n ");
            scanf(" %s", palabra);
            puntero = AGREGAR(palabra, listado, puntero, idioma);
            printf("¿Desea agregar otra palabra distinta? (Y/N)\n ");

            scanf(" %c", &seleccion2);
            seleccion2 = toupper(seleccion2);

            while (seleccion2 != 'Y' && seleccion2 != 'N'){
                printf("Seleccione una opcion correcta!\n ");
                scanf(" %c", &seleccion2);
                seleccion2 = toupper(seleccion2);
            }
        }
        ORDENAR_ALFABETICAMENTE(listado, puntero);
        retorno = 1;
        break;

    case 'M':
        MAS_BUSCADAS(listado, puntero);
        retorno = 1;
        break;

    case 'V':
        //Volver//
        retorno = 0;
        break;

    default:
        break;
    }
    return retorno;
};


/*********************************************************************************
 * Subrutina MENU(listado, puntero)                                              *
 *                                                                               *
 *  Genera el menu principal, con las opciones,traducir del ingles al español    *
 *  traducir del español al ingles y la opcion volver.                           *
 *                                                                               *
 *********************************************************************************/
short MENU(diccionario listado[], int puntero){
    short flag;
    int seleccion;
    printf("-----------------------------------------------------------------------\n");
    printf("\t\tMENU\n 1 - Ingles a español\n 2 - Español a ingles\n 0 - Volver\n ");
    scanf(" %d", &seleccion);
    while(seleccion<0 || seleccion>3){
        printf("Ingrese un valor correcto!\n ");
        fflush(stdin);
        scanf(" %d", &seleccion);
    }
    switch (seleccion){
    case 1:
        while(SUBMENU(listado, puntero, INGLES));//Ingles a español//
        flag = 1;
        break;

    case 2:
        while(SUBMENU(listado, puntero, ESPANOL));//Español a ingles//
        flag = 1;
        break;

    case 0:
        //Volver//
        flag = 0;
        break;

    default:
        break;
    }
    return flag;
};
#endif
//********************************************************************************
