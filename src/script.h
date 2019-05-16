#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED
#include <stdio.h>

#define LINE_SIZE 254                                                           // taille maximale d'une ligne

extern int write_script(char* filename);                                        // ecriture d'un script cimp
extern int load_script(char* filename);                                         // chargement d'un script cimp
extern int edit_script(char* filename);                                         // edition d'un script cimp
extern int rename_script(char* filename,char* new);                             // renommage d'un script cimp
extern int execute_script(char* filename);                                      // execution d'un script cimp

#endif
