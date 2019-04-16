#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
<<<<<<< HEAD
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "script.h"
#include "parser.h"

int write_script(char* filename)
{
     char line[LINE_SIZE];

     int fd = open(filename,O_CREAT|O_TRUNC|O_WRONLY, 0744);

     fprintf(stdout,"CNTRL+D : END OF FILE\n");
     fprintf(stdout,"==========================================================\n");

     while (!feof(stdin))
     {
          memset(line,0,LINE_SIZE);
          fgets(line, LINE_SIZE, stdin);
          int nn = write(fd,line,strlen(line));
          if ( nn < 0 )
               return -1;

     }
     fprintf(stdout,"==========================================================\n");
     close(fd);
=======
#include <sys/stat.h>
#include <fcntl.h>
#include "parser.h"

#define LINE_SIZE 254

int write_script(char* filename)
{
     char line[LINE_SIZE];
     char temp_file[strlen(filename)+6];

     memset(temp_file,0,strlen(filename)+6);
     strcat(temp_file,".temp_");
     strcat(temp_file,filename);
     int fd = open(temp_file,O_CREAT|O_TRUNC|O_WRONLY, 0744);

     fprintf(stdout,"The last line of your script should be \"EOF\".\n");
     fprintf(stdout,"==========================================================\n");

     do
     {
          memset(line,0,LINE_SIZE);
          int n = read(STDIN_FILENO, line, LINE_SIZE);
          if ( n  < 0 )
               return -1;
          else if ( n == 0 || !strcmp(line,"\n") )
               continue;
          else
          {
               if ( strcmp(line,"EOF\n") )
               {
                    int nn = write(fd,line,n);
                    if ( nn < 0 )
                         return -1;
               }
          }
     } while ( strcmp(line,"EOF\n"));

     fprintf(stdout,"==========================================================\n");
>>>>>>> a9b9e50d2ca51f65cc476b4559ed2790982f87e2
     return 0;
}

int load_script(char* filename)
{
     char line[LINE_SIZE];
     if( access( filename, F_OK|R_OK ) != -1 )
     {
<<<<<<< HEAD
          int fd = open(filename,O_RDONLY);
          while ( read(fd,line,LINE_SIZE) > 0 )
          {
               fprintf(stdout,"%s",line);
               memset(line,0,LINE_SIZE);
          }
          return 0;
=======
          int fd = open(filename,RD_ONLY);
          while ( read(fd,line,LINE_SIZE) > 0 )
               printf("%s",line);
               return 0;
>>>>>>> a9b9e50d2ca51f65cc476b4559ed2790982f87e2
     }
     return -1;
}

int edit_script(char* filename)
{
<<<<<<< HEAD
     char s[254];
     int d;
     fprintf(stdout,"1 : nano\n2 : vim\n3 : emacs\n4 : gedit\n5 : pluma\n\nChoose an editor : ");
     scanf("%s",s);
     d = atoi(s);
     if ( d < 1 && d > 5 )
          return -1;
=======
>>>>>>> a9b9e50d2ca51f65cc476b4559ed2790982f87e2
     if( access( filename, F_OK|R_OK ) != -1 )
     {
          int pid = fork();
          if ( ! pid )
          {
<<<<<<< HEAD
               switch (d)
               {
                    case 1 : execlp("nano" , "nano",  filename, (char*) NULL);
                    case 2 : execlp("vim"  , "vim" ,  filename, (char*) NULL);
                    case 3 : execlp("emacs", "emcas", filename, (char*) NULL);
                    case 4 : execlp("gedit", "gedit", filename, (char*) NULL);
                    case 5 : execlp("pluma", "pluma", filename, (char*) NULL);
                    default : exit(-1);
               }
=======
               execlp("nano", "nano", filename, (char*) NULL);
>>>>>>> a9b9e50d2ca51f65cc476b4559ed2790982f87e2
          }
          else if ( pid > 0 )
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
     if( access( filename, F_OK|R_OK ) != -1 )
          return rename(filename,new);
     else
          return -1;
}

<<<<<<< HEAD
int execute_script(char* filename)
{
     char line[LINE_SIZE];
     char** args;
     FILE* fd = NULL;
     command* cmd;
     int ret;
     if( access( filename, F_OK|R_OK ) != -1 )
     {
          fd = fopen(filename,"r");
          while ( fgets(line,LINE_SIZE,fd) && strlen(line) > 1 )
          {
               args= (char**) calloc(SIZE,sizeof(char *));
               cmd = (command*) calloc(1,sizeof(command));
               cmd->files = (char**) calloc(2,sizeof(char*));
               cmd->pixels = (pixel**) calloc(2,sizeof(pixel*));
               cmd->colors = (color**) calloc(2,sizeof(color*));

               line[strlen(line)-1] = '\0';
               ret = parse_by_mode(line,args,cmd);
          }
          fclose(fd);
          return ret;
     }
     fclose(fd);
     return -1;
}
=======
int save_script(char* filename)
{
     char temp_file[strlen(filename)+6];
     memset(temp_file,0,strlen(filename)+6);
     strcat(temp_file,".temp_");
     strcat(temp_file,filename);

     if( access( temp_file, F_OK|R_OK ) != -1 )
          return rename(temp_file,name);
     else
          return -1;
}

int execute_script(char* filename)
{
     char line[LINE_SIZE];
     if( access( filename, F_OK|R_OK ) != -1 )
     {
          int fd = open(filename,RD_ONLY);
          while ( read(fd,line,LINE_SIZE) > 0 )
               if ( ! parse(line) )
                    
               return 0;
     }
     return -1;
}

int main(int argc, char** argv)
{
     char filename[100] = "file.cimp";
     int r =  write_script(filename);
     printf("%d",r);
     return 0;
}
>>>>>>> a9b9e50d2ca51f65cc476b4559ed2790982f87e2
