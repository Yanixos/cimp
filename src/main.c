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

void intHandler(int n);
void intro();
char *replace_str(char *str,char* orig,char* p);
char* cimp_prompt();
char *command_generator (const char *com, int num);
char ** fileman_completion (const char *com, int start, int end);
int initialize_readline();

int main(int argc, char **argv)
{
     //intro();
     BATCH_MODE = 0;
     int ret;
     char* prompt;
     char* line;
     char** args;
     command* cmd;

     signal(SIGINT, intHandler);
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

          if ( !strcmp(line,"") )
               continue;
          ret = parse_by_mode(line,args,cmd);
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
     fprintf(stdout, "\nHave fun editing your images ^_^\n\nPress any key to continue...");
     getchar();
     system("clear");
}

char *replace_str(char *str,char* orig,char* p)
{
     char buffer[SIZE];
     char* rep = strdup("~");

     strncpy(buffer, str, p-str);
     buffer[p-str] = '\0';

     sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));
     return strdup(buffer);
}


char* cimp_prompt()
{
     char *orig = strdup(getenv("HOME"));
     char prompt[SIZE];
     char cwd[254];
     char *p;

     getcwd(cwd, sizeof(cwd));
     sprintf(prompt,"\033[1;31m%s%s\033[0m%s%s",strdup(getenv("USER")),"@cimp:",cwd,"$ ");

     if ( ( p = strstr(prompt, orig) ) != NULL )
          return replace_str(prompt,orig,p);
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
