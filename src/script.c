#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
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
     return 0;
}

int load_script(char* filename)
{
     char line[LINE_SIZE];
     if( access( filename, F_OK|R_OK ) != -1 )
     {
          int fd = open(filename,RD_ONLY);
          while ( read(fd,line,LINE_SIZE) > 0 )
               printf("%s",line);
               return 0;
     }
     return -1;
}

int edit_script(char* filename)
{
     if( access( filename, F_OK|R_OK ) != -1 )
     {
          int pid = fork();
          if ( ! pid )
          {
               execlp("nano", "nano", filename, (char*) NULL);
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
