#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include "subrutinas.h"
/*
#define LONGITUD_LISTA      10000
#define LONGITUD_PALABRA    20
#define ESPANOL     1
#define INGLES      0

typedef struct diccionario{
    char espanol[LONGITUD_PALABRA];
    char ingles[LONGITUD_PALABRA];
    int cant_busquedas;
}diccionario;

diccionario listado[LONGITUD_LISTA];

int INICIAR(diccionario listado[]){
    setlocale(LC_ALL, "");
    int puntero = 0;
    FILE *diccionario;
    diccionario = fopen("espaingl.dat","r");

    while (!feof(diccionario)){
        fscanf(diccionario, "%s\t%s\t%d", listado[puntero].espanol, listado[puntero].ingles, &listado[puntero].cant_busquedas);

        if (strcmp(listado[puntero].espanol, "\0") == 0){
            fscanf(diccionario, "%s\t%s\t%d", listado[puntero].espanol, listado[puntero].ingles, &listado[puntero].cant_busquedas);
        }else{
            puntero++;
        }
    }
    fclose(diccionario);
    return puntero;
};

void FINALIZAR(diccionario listado[], int puntero){
    FILE *diccionario;
    diccionario = fopen("espaingl.dat","w");
    for (int i = 0; i<=puntero; i++){
        fprintf(diccionario, "%s %s %d\n", listado[i].espanol, listado[i].ingles, listado[i].cant_busquedas);
    }
    fclose(diccionario);
};

int COMPARAR(char palabra1[], char palabra2[]){
    char aux1[LONGITUD_PALABRA];
    char aux2[LONGITUD_PALABRA];
    strcpy(aux1, palabra1);
    strcpy(aux2, palabra2);
    for (int i = 0; i<LONGITUD_PALABRA; i++){
            switch (aux1[i]){
                case 'á':
                    aux1[i] = 'a';
                    break;
                case 'é':
                    aux1[i] = 'e';
                    break;
                case 'í':
                    aux1[i] = 'i';
                    break;
                case 'ó':
                    aux1[i] = 'o';
                    break;
                case 'ú':
                    aux1[i] = 'u';
                    break;
                case ' ':
                    aux1[i] = '_';
                    break;
                default:
                    break;
            }
            switch (aux2[i]){
                case 'á':
                    aux2[i] = 'a';
                    break;
                case 'é':
                    aux2[i] = 'e';
                    break;
                case 'í':
                    aux2[i] = 'i';
                    break;
                case 'ó':
                    aux2[i] = 'o';
                    break;
                case 'ú':
                    aux2[i] = 'u';
                    break;
                case ' ':
                    aux1[i] = '_';
                    break;
                default:
                    break;
            }
    }
    return strcmp(aux1,aux2);
};

int BUSCAR(char palabra[], diccionario listado[], int puntero, short idioma){
    int posicion;
    int i = 0;
    int min = 0;
    int max = puntero;
    if (idioma == INGLES){
        while(i<puntero && COMPARAR(palabra, listado[i].ingles) != 0){
            i++;
        }
        if (COMPARAR(palabra, listado[i].ingles)==0){
            posicion = i;
        }else{
            posicion = -1;
        }
    }else{
        i = (min+max)/2;
        while(min<max && COMPARAR(palabra, listado[i].espanol)!=0){
            if(COMPARAR(palabra, listado[i].espanol)<0){
                max = i-1;
            }else{
                min = i+1;
            }
            i = (min+max)/2;
        }
        if (COMPARAR(palabra, listado[i].espanol)==0){
            posicion = i;
        }else{
            posicion = -1;
        }
    }
    return posicion;
};

void ORDENAR_ALFABETICAMENTE(diccionario listado[], int puntero){
    int  j;
    short flag;
    diccionario aux;
    for (int i = 1; i<puntero; i++){
        aux = listado[i];
        j = i-1;
        flag = 1;
        while (flag && j>=0){
            if (COMPARAR(aux.espanol, listado[j].espanol)<0){
                listado[j+1] = listado[j];
                j--;
            }else{
                flag = 0;
            }
        }
        listado[j+1] = aux;
    }
};

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

int AGREGAR(char palabra[], diccionario listado[], int puntero, short idioma){
    char traduccion[LONGITUD_PALABRA];
    char seleccion;
    if(puntero<LONGITUD_LISTA && BUSCAR(palabra, listado, puntero, idioma)<0){
        do{
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
            if (seleccion == 'Y'){
                for (int i = 0; i < strlen(traduccion); i++){
                    if (isspace(traduccion[i])){
                        traduccion[i] = '_';
                    }
                }
                for (int i = 0; i < strlen(palabra); i++){
                    if (isspace(palabra[i])){
                        palabra[i] = '_';
                    }
                }
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
        }while (seleccion == 'N');
    }else if(puntero>=LONGITUD_LISTA){
        printf("Se alcanzo el tope del diccionario, no se pueden agregar mas palabras!\n ");
    }else if(BUSCAR(palabra, listado, puntero, idioma)>=0){
        printf("La palabra buscada ya existe!\n ");
    }
    return puntero;
};

int TRADUCIR(diccionario listado[], int puntero, short idioma){
    int i;
    char seleccion;
    char palabra[LONGITUD_PALABRA];
    printf(" Ingrese la palabra a traducir\n ");
    fflush(stdin);
    gets(palabra);
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

short SUBMENU(diccionario listado[], int puntero, short idioma){
    char seleccion;
    char seleccion2;
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
        do{
            printf("Ingrese la palabra a agregar\n ");
            fflush(stdin);
            gets(palabra);
            puntero = AGREGAR(palabra, listado, puntero, idioma);
            printf("¿Desea agregar otra palabra distinta? (Y/N)\n ");
            scanf(" %c", &seleccion2);
            seleccion2 = toupper(seleccion2);
            while (seleccion2 != 'Y' && seleccion2 != 'N'){
                printf("Seleccione una opcion correcta!\n ");
                scanf(" %c", &seleccion2);
                seleccion2 = toupper(seleccion2);
            }
        }while (seleccion2 == 'Y');
        ORDENAR_ALFABETICAMENTE(listado, puntero);
        retorno = 1;
        break;
    case 'M':
        MAS_BUSCADAS(listado, puntero);
        retorno = 1;
        break;
    case 'V':
        retorno = 0;
        break;
    default:
        break;
    }
    return retorno;
};

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
        while(SUBMENU(listado, puntero, INGLES));
        flag = 1;
        break;
    case 2:
        while(SUBMENU(listado, puntero, ESPANOL));
        flag = 1;
        break;
    case 0:
        flag = 0;
        break;
    default:
        break;
    }
    return flag;
};
*/
int main (void){
    int puntero;
    puntero = INICIAR(listado);
    while (MENU(listado, puntero));
    FINALIZAR(listado, puntero);
}
