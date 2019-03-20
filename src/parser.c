#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <sys/wait.h>
#include "parser.h"

#define die(e) do { fprintf(stderr, "%s(): failed\n", e); exit(EXIT_FAILURE); } while (0);

int check_extension(char* filename)
{
   int err;
   regex_t preg;
   const char *str_regex = "\\.(jpeg|jpg|exif|tiff|gif|bmp|png|ppm|pgm|pbm|pnm|webp|hdr)$";

   err = regcomp (&preg, str_regex, REG_NOSUB | REG_EXTENDED);
   if (err == 0)
   {
      int match;
      match = regexec (&preg, filename, 0, NULL, 0);
      regfree (&preg);
      return match;
   }
   else
    return -1;
}

// takes a command line in string format then it tokenize it and returns an array of strings

void batch_files(command* cmd, char** args, int argc)
{
  int link[2];
  pid_t pid;
  char output[4096+1];
  memset(output, 0, 4096);
  char filenames[SIZE]= "ls -1 ";

  for(int i=2; i < argc; i++)
    strcat(filenames,args[i]);

  if (pipe(link)==-1)
    die("pipe");

   if ((pid = fork()) == -1)
    die("fork");

  if(pid == 0)
  {
    dup2 (link[1], STDOUT_FILENO);
    close(link[0]);
    close(link[1]);
    // find /path/to/dir -regextype posix-extended -regex '/path/to/dir/[0-9]{8,}.*' -exec ls -lh {} \;
    execl("/bin/sh","sh","-c" , filenames, (char *)NULL); // security issues ( command injection )
    die("execl");
  }
  else
  {
    close(link[1]);
    int nbytes = 0;
    cmd->files = calloc(SIZE,sizeof(char*));
    while( (nbytes = read(link[0], output, sizeof(output))) != 0)
    {
        int i = 0;
        for (char *token = strtok(output,"\n"); token != NULL; token = strtok(NULL, "\n"))
        {
          if ( ! check_extension(token))
            fprintf(stderr,"cimp_parser(): skipping: %s\n",token+3);
          else
          {
            fprintf(stdout,"cimp_parser(): parsed: %s\n",token);
            cmd->files[i++] = strdup(token);
          }
        }
        memset(output, 0, 4096);
    }
    wait(NULL);
  }
}

int tokenize(char* line,char** args)
{
    int i = 0;
    for (char *token = strtok(line,SEP); token != NULL; token = strtok(NULL, SEP) )
      args[i++] = strdup(token);

    return i;
}

// checks if the command is syntaxicly correct
int parse_command(char* line, command* cmd)
{
    int cmd_index = -1;
    int i;
    char** args= (char**) calloc(SIZE,sizeof(char *));
    int argc = tokenize(line,args);
    for (  i=0; i < NUM_CMD; i++ )
    {
      if ( ! strcasecmp(args[0],cmd_name[i]) )
      {
        cmd_index = i;
        cmd->name = strdup(cmd_name[i]);
        break;
      }
    }

    if ( i >= NUM_CMD )
      return -1;

    printf("ARGC : %d\n",argc);
    if ( argc >= 2 )
    {
      if ( !strcmp(args[1],"-l") )
      {
        if ( argc < 3 )
        {
          fprintf(stderr,"cimp_parser(): usage: command -l filename [filename..]\n");
          return -1;
        }
        else
        {
          cmd->batch = 1;
          batch_files(cmd,args,argc);
          return cmd_index;
        }
      }
      else
      {
        cmd->batch = 0;
        if ( check_extension(args[1]) )
        {
          fprintf(stderr, "cimp_parser: invalid file extension %s\n", args[1]);
          return -1;
        }
        else
        {
          fprintf(stdout, "cimp_parser(): parsed: %s\n", args[1]);
          cmd->files = (char**) calloc(1,sizeof(char*));
          cmd->files[0] = strdup(args[1]);
          return cmd_index;
        }
      }
    }
    else
    {
      fprintf(stderr, "cimp_parser: usage: command [-l] filename [[filename]...]\n");
      return -1;
    }
}

int main()
{
    command* cmd = (command*) calloc(1,sizeof(command));
    char* line = strdup("load -l *");
    //int  nb = tokenize(line,args);
    //printf("nb:%d\n", nb);
    if (  parse_command(line,cmd) >= 0 )
      printf("well parsed\n");
    else
      printf("bad parsed\n");
}
