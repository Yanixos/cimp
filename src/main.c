#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"

void intHandler(int n);                                                         // fonction qui prend en charge le clique du cntrl+c
void intro();                                                                   // introduction à cimp
char *replace_str(char *str,char* orig,char* p);                                // utiliser pour remplace $HOME par ~
char* cimp_prompt();                                                            // retourne le prompt de cimp
char *command_generator (const char *com, int num);                             // genère la completion automatique
char ** fileman_completion (const char *com, int start, int end);               // retourne les fichiers à suggerer
int initialize_readline();                                                      // intialise la lecture de la ligne de commande

int main(int argc, char **argv)
{
     intro();                                                                   // lancer l'introduction
     BATCH_MODE = 0;                                                            // batch mode est desactivé par défaut
     int ret;
     char* prompt;
     char* line;
     char** args;
     command* cmd;

     signal(SIGINT, intHandler);                                                // associer le signal SIGINT à la fonction intHandler
     initialize_readline ();                                                    // initialiser la completion automatique

     do
     {
          args= (char**) calloc(SIZE,sizeof(char *));
          cmd = (command*) calloc(1,sizeof(command));
          cmd->files = (char**) calloc(2,sizeof(char*));
          cmd->pixels = (pixel**) calloc(2,sizeof(pixel*));
          cmd->colors = (color**) calloc(2,sizeof(color*));

          prompt = cimp_prompt();
          line = readline (prompt);                                             // lecture du commande
          if ( ! line  )                                                        // signal CNTRL+D
               break;                                                           // on sort de la boucle

          if ( !strcmp(line,"") )                                               // ligne vide
               continue;
          ret = parse_by_mode(line,args,cmd,1);                                 // parser la command
          free(cmd);
          free(args);

     } while (1);

     free(line);
     free(prompt);

     fprintf(stdout,"\n");

     exit(ret);
}

void intHandler(int n)
{
     fprintf(stderr,"\n");
     exit(EXIT_FAILURE);
}

void intro()
{
     system("clear");
     fprintf(stdout, "Do you like to get a quick intro ? yes/no: " );
     char c[4];
     fgets(c, 4, stdin);
     while ( strncasecmp(c,"yes",3) && strncasecmp(c,"no",2) )
     {
          fprintf(stderr, "\nWrong choice !\nyes/no: ");
          fgets(c, 4, stdin);
     }
     if ( ! strncasecmp(c,"no",2) )
     {
          system("clear");
          return;
     }

     system("clear");
     fprintf(stdout, "_____________ Welcome to cimp command line interface _____________\n" );
     sleep(1);
     fprintf(stdout, "\nUse: help [command]: to get familliar with the commands\n" );
     sleep(1);
     fprintf(stdout, "\nBatch mode 0: disable the batch mode\n" );
     sleep(1);
     fprintf(stdout, "Batch mode 1: enable the first batch mode: command regex [options1] ... [optionN]\n" );
     sleep(1);
     fprintf(stdout, "Batch mode 2: enable the second batch mode: command file1 ... fileN [options1] ... [optionN]\n" );
     sleep(1);
     fprintf(stdout, "By default : batch mode is disabled\n");
     sleep(1);
     fprintf(stdout, "\nHave fun editing your images ^_^\n\nPress any key to continue...\n");
     getchar();
     getc(stdin);
     system("clear");
}

char *replace_str(char *str,char* orig,char* p)
{
     char buffer[SIZE];
     char* rep = strdup("~");

     strncpy(buffer, str, p-str);
     buffer[p-str] = '\0';

     sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));
     free(orig);
     free(rep);
     return strdup(buffer);
}


char* cimp_prompt()
{
     char *orig = strdup(getenv("HOME"));                                       // recuperer le chemin de repertoire home
     char prompt[SIZE];
     char cwd[254];
     char *p;

     getcwd(cwd, sizeof(cwd));                                                  // recuperer le chemin du repertoire actuel
     sprintf(prompt,"\033[1;31m%s%s\033[0m%s%s",strdup(getenv("USER")),"@cimp:",cwd,"$ ");     //créer le prompot

     if ( ( p = strstr(prompt, orig) ) != NULL )                                // si le prompt contient le chemin du home
          return replace_str(prompt,orig,p);                                    // on le remplace par ~
     else
          return strdup(prompt);
}

char *command_generator (const char *com, int num)
{
     static int indice, len;
     char *completion;

     if (num == 0)
     {
          indice = 0;
          len = strlen(com);
     }

     while (indice < NUM_PARSE)
     {
          completion = cmd_name[indice++];

        if (strncmp (completion, com, len) == 0)
          return strdup(completion);
    }

    return NULL;
}

char ** fileman_completion (const char *com, int start, int end)
{
     char **matches;
     matches = (char **)NULL;

     if (start == 0)
          matches = rl_completion_matches (com, command_generator);

     return (matches);
}

int initialize_readline()
{
     rl_attempted_completion_function = fileman_completion;
     return 0;
}
