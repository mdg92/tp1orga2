#include "tdt.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main (void){

    tdt* tabla = tdt_crear("pepe");
    FILE* arch;
    arch=fopen ("salida.txt", "w");
    uint8_t cla[3]={0,0,0};
    uint8_t sig[15]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    maxmin* o=tdt_obtenerMaxMin(tabla);
    tdt_agregar(tabla, cla, sig);
    bloque b1={{0,0,0},{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}};
    bloque* b[2]={&b1,0};
    tdt_borrarBloques(tabla, b);
    tdt_imprimirTraducciones(tabla, arch);
    fprintf(arch, "max cla %02X%02X%02X\n", o->max_clave[0],o->max_clave[1],o->max_clave[2] );
    free(o);
    fclose (arch);
    tdt_destruir(&(tabla));

    
    return 0;    
}
