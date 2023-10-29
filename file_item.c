/*
 * @fileoverview    {FileName}
 *
 * @version         2.0
 *
 * @author          Dyson Arley Parra Tilano <dysontilano@gmail.com>
 *
 * @copyright       Dyson Parra
 * @see             github.com/DysonParra
 *
 * History
 * @version 1.0     Implementation done.
 * @version 2.0     Documentation added.
 */
#include "file_item.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

CALLING void newFileItem(FileList* list, char* name);
CALLING int compareArrays(char* array1, char* array2);

#if (!defined STATIC_LIB_) && (!defined NO_DLL_MAIN_)
/**
 * Entrada principal de la dll (creado si no se indica que es biblioteca estática)
 *
 * @param instance identificador del módulo.
 * @param reason   razón de la llamada.
 * @param reserved argumento reservado.
 * @return {@code TRUE} si se completó exitosamente la llamada, caso contrario {@code FALSE}.
 */
BOOL APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
    /*
     * Evalúa la razón de la llamada a la dll.
     */
    switch (reason) {
        /*
         * Se cargó la dll en el proceso actual.
         */
        case DLL_PROCESS_ATTACH:
            // printf("%s\n", "DLL_PROCESS_ATTACH");
            break;

        /*
         * Se liberó la dll en el proceso actual.
         */
        case DLL_PROCESS_DETACH:
            // printf("%s\n", "DLL_PROCESS_DETACH");
            break;

        /*
         * Se cargó la dll en un nuevo hilo.
         */
        case DLL_THREAD_ATTACH:
            // printf("%s\n", "DLL_THREAD_ATTACH");
            break;

        /*
         * Se liberó la dll en el nuevo hilo.
         */
        case DLL_THREAD_DETACH:
            // printf("%s\n", "DLL_THREAD_DETACH");
            break;
    }

    return TRUE;
}
#endif

/**
 * FIXME: Description of {@code compareArrays}.
 * Compara dos arrays sin diferenciar entre mayúsculas y minúsculas.
 */
CALLING int compareArrays(char* array1, char* array2) {
    int result = 1; // Variable para almacenar el resultado (por defecto 0).
    char c1 = ' ';  // Crea char para almacenar cada letra del array1.
    char c2 = ' ';  // Crea char para almacenar cada letra del array2.

    if (array1 != NULL && array2 != NULL && (strlen(array1) == strlen(array2))) {   // Si ninguno de los array es NULL y tienen igual longitud.
        for (int i = 0; i < (int)(strlen(array1)); i++) {                           // Compara ambos arrays.
            c1 = array1[i];                                                         // Almacena la letra actual del array1.
            c2 = array2[i];                                                         // Almacena la letra actual del array2.

            if (((c1 >= 65 && c1 <= 90) && (c1 != c2 && c1 != c2 - 32))             // Si encuentra una letra mayúscula en array1 y es diferente a la letra actual de array2 en minúscula.
                || ((c1 >= 97 && c1 <= 122) && (c1 != c2 && c1 != c2 + 32))         // Si encuentra una letra minúscula en array1 y es diferente a la letra actual de array2 en mayúscula.
                || (c1 != c2)) {                                                    // Si los arrays no tienen el mismo caracter.
                result = 0;                                                         // Indica que los array no son iguales.
                break;                                                              // Sale del for.
            }
        }
    } else                                                                          // Si alguno de los arrays etá vacío o no tienen igual longitud.
        result = 0;                                                                 // Indica que los arrays son diferentes.

    return result;                                                                  // Devuelve el valor de result.
}

/**
 * FIXME: Description of {@code newFileItem}.
 * Agrega un FileItem a una lista de FileItem.
 */
CALLING void newFileItem(FileList* list, char* name) {
    FileItem* nuevo;                                        // Declara apuntador a un FileItem.
    nuevo = (FileItem*)malloc(sizeof(FileItem));            // reserva espacio en el heap para el FileItem.

    nuevo->name[0] = 0;                                     // Pone el campo name de "nuevo" en la posición cero en 0.

    sprintf(nuevo->name, "%s", name);                       // Copia el parámetro name al campo name de "nuevo".
    nuevo->next = NULL;                                     // Asigna a nuevo su siguiente FileItem como null.

    if (list->first == NULL) {                              // Si la lista está vacía.
        list->first = nuevo;                                // El nuevo FileItem es el primero de la lista.
        list->last = nuevo;                                 // El nuevo FileItem es el último de la lista.
    } else {                                                // Si no está vacía la lista.
        list->last->next = nuevo;                           // Liga el nuevo FileItem con el último de la lista.
        list->last = nuevo;                                 // Indica que "nuevo" es ahora el último FileItem de la lista.
    }
}

/**
 * FIXME: Description of {@code makeFileList}.
 * Crea una lista de FileItem con todos los archivos en "path" terminados en los formatos indicados por "formats".
 */
LANGUAGE DLLIMPORT CALLING FileList* makeFileList(char* path, char* formats) {
    FileList* list = (FileList*)malloc(sizeof(FileList));               // Crea una lista que tendrá los archivos.
    list->first = NULL;                                                 // Indica que la lista de archivos está vacía.

    FileList* format = (FileList*)malloc(sizeof(FileList));             // Crea una lista que tendrá los fomatos.
    format->first = NULL;                                               // Indica que la lista de formatos está vacía.
    char formatAux[20];                                                 // Crea array para almacenar cada formato encontrado.

    int start = -1;                                                     // Crea variable para indicar el inicio de cada formtao en el array formatos.

    for (int i = 0; i < (int)strlen(formats); i++)                      // Recorre el array con los formatos.
        if (formats[i] == '.')                                          // Si encuentra un '.'.
            start = i;                                                  // Indica el inicio de un formato.
    elif ((formats[i] == ' ' || formats[i] == '\t') && start != -1) {   // Si encuentra un espacio o tabulación (fin de un formato) y ya se había indicado un inicio.
        memcpy(formatAux, &formats[start], i - start);                  // Copia el formato en formatAux.
        formatAux[i - start] = 0;                                       // Marca el fin de la cadena en el formato actual.
        newFileItem(format, formatAux);                                 // Agrega un nuevo FileItem a la lista de formatos.
        start = -1;                                                     // Reinicia el inicio de un formato.
    }
    elif (i == (int)strlen(formats) - 1 && start != -1) {               // Si encuentra el fin del array (fin de un formato) y ya se había indicado un inicio.
        memcpy(formatAux, &formats[start], i - start + 1);              // Copia el formato en formatAux.
        formatAux[i - start + 1] = 0;                                   // Marca el fin de la cadena en el formato actual.
        newFileItem(format, formatAux);                                 // Agrega un nuevo FileItem a la lista de formatos.
        start = -1;                                                     // Reinicia el inicio de un formato.
    }

    // printf("\nFormats:\n");                                          // Muestra mensaje.
    // printFileList(format);                                           // Muestra la lista de formatos.

    char buf[1000];                                                     // Crea array para ejecutar comandos.
    sprintf(buf, "dir \"%s\" /B /A-D > list.tmp", path);                // Almacena el comando para crear la lista de archivos en buf.
    // printf("'%s'\n", buf);                                           // Muestra el comando para crear el archivo con la lista de archivos.
    system(buf);                                                        // Crea el archivo con la lista de archivos.
    FILE* fp = fopen("list.tmp", "r");                                  // Abre "list.tmp".
    char fileName[1024] = "";                                           // Crea array para almacenar cada archivo.
    FileItem* aux = NULL;                                               // Crea un apuntador a un FileItem.

    while (!feof(fp)) {                                                 // Mientras no se recorra todo list.tmp.
        fgets(fileName, 1024, fp);                                       // Almacena la linea actual en filename.

        if (feof(fp))                                                   // Si termino de reorrer el archivo.
            break;                                                      // Sale del ciclo.

        strtok(fileName, "\n");                                         // Pone un '0' en fileName cuando encuentre un salto de línea '10'.
        aux = format->first;                                            // A aux le lleva la posición de memoria del primer FileItem de la lista de formatos.

        while (aux != NULL) {                                           // Recorre la lista de formatos.
            if (compareArrays(getFormat(fileName), aux->name)) {        // Si el formato del archivo es igual a alguno de la lista de formatos.
                newFileItem(list, fileName);                            // agrega FileItem a lista.
                break;                                                  // Sale del while.
            }
            aux = aux->next;                                            // Pasa al siguiente formato.
        }
    }

    fclose(fp);                                                         // Cierra "list.tmp".
    system("DEL list.tmp");                                             // Elimina "list.tmp".
    free(format);                                                       // Libera la memoria usada por format.

    return list;                                                        // Devuelve lista de FileItem.
}

/**
 * FIXME: Description of {@code printFileList}.
 * printList muestra los elementos de una lista ligada parámetro en pantalla.
 */
LANGUAGE DLLIMPORT CALLING void printFileList(FileList* list) {
    FileItem* aux = NULL;                       // Crea un apuntador a un nodo.

    if (list->first != NULL)                    // Si la lista no está vacía.
        aux = list->first;                      // A aux le lleva la posición de memoria del primer nodo de la lista.
    else                                        // Si la lista está vacía.
        printf("List is void\n");               // Imprime que está vacía.

    while (aux != NULL) {                       // Recorre la lista ligada.
        printf("\"%s\"\n", aux->name);          // Imprime el nodo actual.
        aux = aux->next;                        // Pasa al siguiente nodo.
    }
    // printf("\n");                            // Imprime salto de línea.
}

/**
 * FIXME: Description of {@code getName}.
 * Obtiene el nombre de una cadena con la ruta absoluta o relativa de un archivo.
 */
LANGUAGE DLLIMPORT CALLING char* getName(char* file) {
    char* name = file;                                      // Crea apuntador que tendrá la posición del nombre del array.
    if (file != NULL)                                       // Si el array no es NULL.
        for (int i = (int)(strlen(file)); i >= 0; i--)      // Recorre file desde la última posición.
            if (file[i] == '/' || file[i] == '\\') {        // Si encuentra indicador de ruta padre.
                name = &file[i + 1];                        // Indica que el nombre del archivo es en la posición siguiente.
                break;                                      // Sale del for.
            }

    return name; // Devuelve la posición donde encontró el nombre del archivo.
}

/**
 * FIXME: Description of {@code getFormat}.
 * Obtiene el formato de una cadena con la ruta de un archivo.
 */
LANGUAGE DLLIMPORT CALLING char* getFormat(char* file) {
    char* format = NULL;                                    // Crea apuntador que tendrá la posición del formato del array.
    if (file != NULL)                                       // Si el array no es NULL.
        for (int i = (int)(strlen(file)); i >= 0; i--)      // Recorre file desde la última posición.
            if (file[i] == '.') {                           // Si encuentra un punto.
                format = &file[i];                          // Indica que el formato del archivo es en la posición actual.
                break;                                      // Sale del for.
            }

    return format; // Devuelve la posición donde encontró el formato del archivo.
}

/**
 * FIXME: Description of {@code getFullPath}.
 * Obtiene la ruta absoluta del archivo indicado por filePath.
 */
LANGUAGE DLLIMPORT CALLING char* getFullPath(char* filePath) {
    char* path;                                         // Crea variable que tendrá el directorio de filePath.
    path = (char*)malloc(260 * sizeof(char));           // Inicializa path con la cantidad máxima de caracteres de la ruta de un archivo.
    getcwd(path, 260);                                  // Asigna a path el directorio de trabajo actual en la consola.

    // printf ("Exec = %s\n", filePath);                // Muestra la ruta de filePath.
    // printf ("Work = %s\n", path);                    // Muestra el directorio de trabajo.

    if (filePath[1] != ':')                             // Si filePath tiene una ruta relativa.
        sprintf(path, "%s\\%s", path, filePath);        // Concatena la ruta de trabajo con la ruta de filePath.
    else                                                // Si filePath tiene una ruta absoluta.
        sprintf(path, "%s", filePath);                  // Lleva la ruta absoulta a path.

    // printf("Full = %s\n", path);                     // Muestra la ruta absoluta de filePath.

    return path;                                        // Devuelve la ruta absoluta de filePath.
}

/**
 * FIXME: Description of {@code getFatherPath}.
 * Obtiene la ruta absoluta del directorio padre dle archivo indicado por filePath.
 */
LANGUAGE DLLIMPORT CALLING char* getFatherPath(char* filePath) {
    char* path = getFullPath(filePath); // Crea array con la ruta absoluta de filePath.
    // printf("AbPath = %s\n", path);               // Muestra la ruta absoluta de filePath.

    for (int i = strlen(path); i >= 0; i--)         // Recorre path.
        if (path[i] == '\\' || path[i] == '/') {    // Si encuentra fin de directorio en path.
            path[i] = 0;                            // Marca el fin de la cadena en path.
            break;                                  // Sale del for.
        }

    // printf("Father = %s\n", path);               // Muestra la ruta absoluta del directorio padre.

    return path;                                    // Devuelve el directorio de filePath.
}

/**
 * FIXME: Description of {@code getFileSize}.
 * Obtiene el tamaño de un archivo. El resultado se genera en bytes, divisios indica cuantas veces se dividirá por 1024. (1 = KB, 2 = MB, 3 = GB).
 */
LANGUAGE DLLIMPORT CALLING double getFileSize(char* fileName, int divisions) {
    char buf[BUFSIZ];                                                                   // Crea array para almacenar comandos.
    sprintf(buf, "for %ci in (\"%s\") do @echo %c~zi > size.tmp", 37, fileName, 37);    // Crea un archivo con el tamaño de fileName (en bytes).
    system(buf);                                                                        // Ejecuta la creación del archivo que contendrá el tamaño (size.tmp).

    char line[256];                                                                     // Crea line que tendrá el contenido de size.tmp.
    FILE* fp = fopen("size.tmp", "r+");                                                 // Abre size.tmp.
    fgets(line, 256, fp);                                                               // Almacena la ruta actual en source.
    fclose(fp);                                                                         // Cierra "size.tmp".
    system("DEL size.tmp");                                                             // Borra "size.tmp".

    double bytes = atof(line);                                                          // A size le asigna el valor numérico de line.
    // printf("%s", line);                                                              // Muestra el contenido de line.
    // printf("%I64d KB\n", bytes);                                                     // Muestra el contenido de size.

    for (int i = 1; i <= divisions; i++)                                                // Divide el tamaño por 1024 tantas veces como diga divisions.
        bytes /= 1024.0;

    return bytes;                                                                       // Devuelve size.
}
