#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include "subrutinas.h"

int main (void){
    int puntero;
    puntero = INICIAR(listado);
    while (MENU(listado, puntero));
    FINALIZAR(listado, puntero);
}
