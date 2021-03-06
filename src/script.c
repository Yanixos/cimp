#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include "script.h"
#include "parser.h"
#include "win_img.h"

int write_script(char* filename)
{
     char line[LINE_SIZE];

     int fd = open(filename,O_CREAT|O_TRUNC|O_WRONLY, 0744);                    // création du fichier

     fprintf(stdout,"CTRL+D : END OF FILE\n");
     fprintf(stdout,"==========================================================\n");

     while (!feof(stdin))                                                       // cntrl+d est la fin du fichier
     {
          memset(line,0,LINE_SIZE);
          fgets(line, LINE_SIZE, stdin);                                        // lire une ligne
          int nn = write(fd,line,strlen(line));                                 // ecrire la ligne
          if ( nn < 0 )
               return -1;

     }
     fprintf(stdout,"==========================================================\n");
     close(fd);
     return 0;
}

int load_script(char* filename)
{
     char line[LINE_SIZE];
     if( access( filename, F_OK|R_OK ) != -1 )                                  // si le fichier existe
     {
          int fd = open(filename,O_RDONLY);                                     // on l'ouvre
          while ( read(fd,line,LINE_SIZE) > 0 )                                 // on lit le contenu
          {
               fprintf(stdout,"%s",line);                                       // on l'affiche
               memset(line,0,LINE_SIZE);
          }
          return 0;
     }
     return -1;
}

int edit_script(char* filename)
{
     char s[254];
     int d;

     if( access( filename, F_OK|R_OK ) != -1 )                                  // si le fichier existe
     {
          fprintf(stdout,"1 : nano\n2 : vim\n3 : emacs\n4 : gedit\n5 : pluma\n\nChoose an editor : ");
          scanf("%s",s);                                                        // choisir un editeur
          d = atoi(s);

          while ( d < 1 || d > 5 )                                              // si choix incorrecte
          {
               fprintf(stderr, "\nWrong choice !\n\n1 : nano\n2 : vim\n3 : emacs\n4 : gedit\n5 : pluma\n\nChoose an editor : " );
               scanf("%s",s);
               d = atoi(s);
          }

          int pid = fork();                                                     // si choix correcte
          if ( ! pid )
          {
               switch (d)                                                       // lancer l'editeur
               {
                    case 1 : execlp("nano" , "nano",  filename, (char*) NULL);
                    case 2 : execlp("vim"  , "vim" ,  filename, (char*) NULL);
                    case 3 : execlp("emacs", "emcas", filename, (char*) NULL);
                    case 4 : execlp("gedit", "gedit", filename, (char*) NULL);
                    case 5 : execlp("pluma", "pluma", filename, (char*) NULL);
                    default : return -1;
               }
          }
          else if ( pid > 0 )                                                   // attente la fin d'edition
          {
               wait(NULL);
               return 0;
          }
          return -1;
     }
     return -1;
}

int rename_script(char* filename,char* new)
{
     if( access( filename, F_OK|R_OK ) != -1 )                                  // si le fichier existe
          return rename(filename,new);                                          // renommer le fichier
     else
          return -1;
}

int execute_script(char* filename)
{
     char line[LINE_SIZE];
     char** args;
     FILE* fd = NULL;
     command* cmd;
     int ret;
     if( access( filename, F_OK|R_OK ) != -1 )                                  // si le fichier existe
     {
          fd = fopen(filename,"r");                                             // ouvrir le fichier
          while ( fgets(line,LINE_SIZE,fd) && strlen(line) > 1 )                // lire le fichier
          {
               args= (char**) calloc(SIZE,sizeof(char *));
               cmd = (command*) calloc(1,sizeof(command));
               cmd->files = (char**) calloc(2,sizeof(char*));
               cmd->pixels = (pixel**) calloc(2,sizeof(pixel*));
               cmd->colors = (color**) calloc(2,sizeof(color*));

               line[strlen(line)-1] = '\0';                                     // enlevé le saut de ligne de la commande
               ret = parse_by_mode(line,args,cmd,0);                            // parser la ligne en specifiant le flag 0 pour ne pas ajouter à l'historique
          }
          fclose(fd);
          return ret;
     }
     fclose(fd);
     return -1;
}


int count_lines (char* filename)
{
     FILE* fp;
     if ( ( fp = fopen(filename,"r") ) == NULL)
          return -1;

     int count = 0;

     for (char c = getc(fp); c != EOF; c = getc(fp))
          if (c == '\n')
               count = count + 1;

     fclose(fp);
     return count;
}


int undo_redo(char* filename, int ur_flag)
{

     char dest[254];
     sprintf(dest,"%s/.%s_cimphistory",getenv("HOME"),filename);

     int nb = count_lines(dest);

     int id = get_id_name(filename);
     int pos = get_index_by_id(id);

     if( nb >= 2 && access( filename, F_OK|R_OK ) != -1 )
     {

          if ( ! ur_flag )
          {
               if ( abs(pos) < nb-1 )
               {
                    pos--;
                    set_index_by_id(id,pos);
               }
          }
          else
          {
               if ( pos < 0 )
               {
                    pos++;
                    set_index_by_id(id,pos);
               }
          }
          char cmd[1024];
          if ( pos <= 0 )
          {
               if ( pos == 0 )
                    pos = nb;

               sprintf(cmd,"rm %s && \
                            cp ~/.%s_cimpbackup %s && \
                            head -n %d ~/.%s_cimphistory > ~/.temp.cimp && \
                            sed -i 's/open %s/open %s -o %s/' ~/.temp.cimp", filename,filename,filename,pos,filename,filename,filename,filename);

               system(cmd);
               char t[254];
               sprintf(t,"%s/.temp.cimp",getenv("HOME"));
               execute_script(t);
               remove(t);
          }
     }
     return -1;
}
