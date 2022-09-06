/*
 * @fileoverview {FileName} se encarga de realizar tareas especificas.
 *
 * @version             1.0
 *
 * @author              Dyson Arley Parra Tilano <dysontilano@gmail.com>
 * Copyright (C) Dyson Parra
 *
 * @History v1.0 --- La implementacion de {FileName} fue realizada el 31/07/2022.
 * @Dev - La primera version de {FileName} fue escrita por Dyson A. Parra T.
 */
#include "file_item.h"
#include "tester.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Entrada principal del sistema.
 *
 * @param argc cantidad de argumentos pasados en la linea de comandos.
 * @param argv argumentos de la linea de comandos.
 * @return {0} si el programa se ejecutó correctamente.
 */
int main(int argc, char** argv) {

    FileList* files = makeFileList((char*)"object", (char*)".avi .mp4 .dll .c .jar .html .xml .mkv .ogg .wav .rtf .exe .o");    // Llena la lista de archivos indicados.
    printf("\nFILES:\n");                                                       // Muestra mensaje.
    printFileList(files);                                                       // Muestra la lista de archivos.

    char* full = getFullPath(files->first->name);
    char* fullName = getName(full);
    char* relName = getName(files->first->name);
    char* format = getFormat(files->first->name);

    printf("\n\n");
    printf("'%s'\n", full);
    printf("'%s'\n", files->first->name);
    printf("'%s'\n", fullName);
    printf("'%s'\n", relName);
    printf("'%s'\n\n", format);

    /*-
    FileItem* aux = files->first;
    while (aux != NULL) {
        deleteFormat(aux->name);
        aux = aux->next;
    }
    printf("\nFILES:\n");                                                       // Muestra mensaje.
    printFileList(files);                                                       // Muestra la lista de archivos.
    */

    /*-
    printf("\"%s\"\n",(char*)getFullPath(argv[0]));
    printf("\n");
    //printf("\"%s\"\n",(char*)getFullPath((char*)"C:\\Users\\Linux\\Desktop\\C projects\\FileItem\\tester.exe"));
    */

    /*-
    printf("\"%s\"\n",(char*)getFatherPath(argv[0]));
    //printf("\n");
    //printf("\"%s\"\n",(char*)getFatherPath((char*)"C:\\Users\\Linux\\Desktop\\C projects\\FileItem\\tester.exe"));
    */

    printf("Name: \"%s\"\n", __FILE__);
    printf("Size: %f KB\n", getFileSize(__FILE__, 1));
    printf("Size: %f B\n", getFileSize(__FILE__, 0));
    //printf("Size: %f B\n", getFileSize("C:\\Users\\Linux\\Desktop\\error.png", 0));

    return 0;
}
