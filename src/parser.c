#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <sys/wait.h>
#include "parser.h"

#define die(e) do { fprintf(stderr, "%s(): failed\n", e); return -1; } while (0);

char* help[] =
{
     "cimp_parser(): syntax error\nopen syntax: open filename.EXT [-o filename_.EXT]\n",
     "cimp_parser(): syntax error\nsave syntax: save filename.EXT\n",
     "cimp_parser(): syntax error\nexport syntax: export filename.EXT -o filename.EXT\n",
     "cimp_parser(): syntax error\nset_bg syntax: set_bg filename.EXT -c COLOR\n",
     "cimp_parser(): syntax error\nselect syntax: select filename.EXT [-m ADD|SUB|OVERWRITE] [-p (X1,Y1) (X2,Y2)] [-c COLOR] [-t]\n",
     "cimp_parser(): syntax error\nunselect syntax: unselect filename.EXT [-p (X1,Y1) (X2,Y2)]\n",
     "cimp_parser(): syntax error\ncopy syntax: copy filename.EXT\n",
	"cimp_parser(): syntax error\ncut syntax: cut filename.EXT [-f COLOR]\n",
	"cimp_parser(): syntax error\npaste syntax: paste filename.EXT -p (X,Y)\n",
	"cimp_parser(): syntax error\nsymetric syntax: symetric filename.EXT -d V|H\n",
	"cimp_parser(): syntax error\nrotate syntax: rotate filename.EXT -a ANGLE\n",
	"cimp_parser(): syntax error\nscale syntax: scale filename.EXT -p (100,200) (300,400) [-c]\n",
	"cimp_parser(): syntax error\nresize syntax: resize filename.EXT -s (X,Y)\n",
	"cimp_parser(): syntax error\nfill syntax: fill filename.EXT -c COLOR\n",
	"cimp_parser(): syntax error\nreplace syntax: replace filename.EXT COLOR1 COLOR2 [-t]\n",
	"cimp_parser(): syntax error\nnegative syntax: negative filename.EXT\n",
	"cimp_parser(): syntax error\ngray syntax: gray filename.EXT\n",
	"cimp_parser(): syntax error\nblack_white syntax: black_white filename.EXT\n",
	"cimp_parser(): syntax error\nbrightness syntax: brightness filename.EXT\n",
	"cimp_parser(): syntax error\ncontrast syntax: contrast filename.EXT\n",
	"cimp_parser(): syntax error\nwrite_script syntax: write_script filename.cimp\n",
	"cimp_parser(): syntax error\nload_script syntax: load_script filename.cimp\n",
	"cimp_parser(): syntax error\nedit_script syntax: edit_script filename.cimp\n",
	"cimp_parser(): syntax error\nsave_script syntax: save_script filename.cimp\n",
	"cimp_parser(): syntax error\nrename_script syntax: rename_script filename.cimp -o filename_.cimp\n",
	"cimp_parser(): syntax error\nexecute_script syntax: execute_script filename.cimp\n",
     "cimp_parser(): syntax error\nundo syntax: undo filename.EXT\n",
     "cimp_parser(): syntax error\nredo syntax: redo filename.EXT\n",
     "cimp_parser(): syntax error\nmodify_bpc syntax: modify_bpc filename.EXT -m 1|2|3\n",
     "cimp_parser(): syntax error\nart_effect syntax: art_effect filename.EXT -m 1|2|3\n",
};

int check_extension(char* filename)
{

   if ( strlen(filename) < 5 )
     return -1;

   const char *str_regex = "\\.(jpeg|jpg|exif|tiff|gif|bmp|png|ppm|pgm|pbm|pnm|webp|hdr)$";
   regex_t preg;
   if ( !(regcomp (&preg, str_regex, REG_NOSUB | REG_EXTENDED)) )
   {
      int match;
      match = regexec (&preg, filename, 0, NULL, 0);
      regfree (&preg);
      return match;
   }
   else
    return -1;
}

int check_pixel(char* arg, pixel* p)
{
     int end = strlen(arg)-1;
     char* e = strchr(arg, ',');
     int index = e != NULL ? (int)(e - arg) : 0;

     if ( arg[0] != '(' || arg[end] != ')' || index <= 1 || index == end - 1 || index == end  || end < 4 )
          return -1;
     else
     {
          char s1[24];
          char s2[24];
          strncpy(s1,arg+1,index-1);
          strncpy(s2,arg+index+1,end-index+1);
          p->x = atoi(s1);
          p->y = atoi(s2);
          return 0;
     }
}

int tokenize(char* line,char** args)
{
    int i = 0;
    for (char *token = strtok(line,SEP); token != NULL; token = strtok(NULL, SEP) )
          args[i++] = strdup(token);
    return i;
}

/*
int batch_files(command* cmd, char** args)
{
     int link[2];
     pid_t pid;
     char output[4096+1];
     memset(output, 0, 4096);
     char ls_regex[SIZE]= "ls -1 ";

     for(int i=2; i < cmd->argc; i++)
          strcat(ls_regex,args[i]);

     if (pipe(link)==-1)
          die("pipe");

     if ((pid = fork()) == -1)
          die("fork");

     if(pid == 0)
     {
          dup2 (link[1], STDOUT_FILENO);
          dup2 (link[1], STDERR_FILENO);
          close(link[0]);
          close(link[1]);
          execl("/bin/sh","sh","-c" , ls_regex, (char *)NULL); // security issues ( command injection )
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
               if ( check_extension(token))
                    return -1;
               else
               {
                 cmd->files[i++] = strdup(token);
               }
             }
             memset(output, 0, 4096);
          }
          wait(NULL);
          return 0;
     }
}
*/

void print_args(int argc, char** args)
{
     for(int i=0; i<argc; i++)
          printf("|%s",args[i]);
     printf("\n");
}

int parse(char* line, command* cmd)
{
     int i;
     char** args= (char**) calloc(SIZE,sizeof(char *));
     cmd->argc = tokenize(line,args);
     for (  i=0; i < NUM_CMD; i++ )
     {
          if ( ! strcasecmp(args[0],cmd_name[i]) )
          {
               return parse_func[i](args,cmd,i) ;
          }
     }
     printf("cimp_parser(): unknown command\n");
     return -1;
}

int parse_open(char** args, command* cmd, int index)
{
     // open filename.EXT [-o filename_.EXT]
     if ( (cmd->argc != 2 && cmd->argc != 4) ||  check_extension(args[1]) || \
          (cmd->argc == 4 && ( strcmp(args[2],"-o") || check_extension(args[3]) ) ) )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }
     if (cmd->argc == 2)
     {
          cmd->option = 0;
          cmd->files[0] = strdup(args[1]);
          return 0;
     }
     else if ( cmd->argc == 4 && !strcmp(args[2],"-o") && !check_extension(args[3]) )
     {
          cmd->option = 1;
          cmd->files[0] = strdup(args[1]);
          cmd->files[1] = strdup(args[3]);
          return 0;
     }
     else
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }
}

int parse_save(char** args, command* cmd, int index)
{
     //save filename.EXT\n"
     if ( cmd->argc != 2 || check_extension(args[1]) )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }
     else
     {
          cmd->files[0] = strdup(args[1]);
          return 0;
     }
}

int parse_export(char** args, command* cmd, int index)
{
     // export filename.EXT -o filename.EXT
     if ( cmd->argc != 4 || check_extension(args[1]) || check_extension(args[3]) || strcmp(args[2],"-o") )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }
     else
     {
          cmd->files[0] = strdup(args[1]);
          cmd->files[1] = strdup(args[3]);
          return 0;
     }
}

int parse_set_bg(char** args, command* cmd, int index)
{
     // set_bg filename.EXT -c COLOR
     if ( cmd->argc != 4 || check_extension(args[1]) || strcmp(args[2],"-c") )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }
     else
     {
          cmd->files[0] = strdup(args[1]);
          cmd->color = strdup(args[3]);
          return 0;
     }
}

int parse_select(char** args, command* cmd, int index)
{
     //select filename.EXT [-m OVERWRITE|ADD|SUB] [-p (X1,Y1) (X2,Y2)] [-c COLOR] [-t]
     cmd->p1 = (pixel*) calloc(1,sizeof(pixel));
     cmd->p2 = (pixel*) calloc(1,sizeof(pixel));
     pixel *t1 = cmd->p1;
     pixel *t2 = cmd->p2;

     if (
          ( (cmd->argc <= 1) || (cmd->argc == 3  && strcmp(args[2],"-t")) ) || \
          ( (cmd->argc >= 2) && (!strcmp(args[cmd->argc-2],"-p")) ) || \
          ( (cmd->argc >= 2) && (check_extension(args[1])) )  || \
          ( (cmd->argc >= 4) && (!strcmp(args[2],"-m")) && (strcasecmp(args[3],"ADD")) && (strcasecmp(args[3],"SUB")) && (strcasecmp(args[3],"OVERWRITE")) ) || \
          ( (cmd->argc >= 5) && (!strcmp(args[2],"-p")) && ( (check_pixel(args[3],t1)) || (check_pixel(args[4],t2))) )|| \
          ( (cmd->argc >= 7 )&& (!strcmp(args[4],"-p")) && ( (check_pixel(args[5],t1)) || (check_pixel(args[6],t2))) )
        )

     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }

     cmd->option = 0;
     cmd->files[0] = strdup(args[1]);
     if ( cmd->argc >= 4 &&  ! strcmp(args[2],"-m") )
     {
          if ( !strcasecmp(args[3],"OVERWRITE"))
               cmd->option += 1;
          else if ( !strcasecmp(args[3],"ADD"))
               cmd->option += 2;
          else if ( !strcasecmp(args[3],"SUB"))
               cmd->option += 3;
          else
          {
               cmd = NULL;
               fprintf(stderr,"%s",help[index]);
               return -1;
          }
     }

     if ( cmd->argc >= 4 &&  !strcmp(args[2],"-p") )
          cmd->option += 10;

     if ( cmd->argc >= 3 && !strcmp(args[cmd->argc -1],"-t") )
          cmd->option += 100;

     if ( cmd->argc >= 4 && !strcmp(args[cmd->argc-2],"-c") )
          cmd->color = strdup(args[cmd->argc-1]);

     if ( cmd->argc >= 4 && !strcmp(args[cmd->argc-3],"-c") )
          cmd->color = strdup(args[cmd->argc-2]);

     return 0;

}
int parse_unselect(char** args, command* cmd, int index)
{
     //unselect filename.EXT [-p (X1,Y1) (X2,Y2)]
     cmd->p1 = (pixel*) calloc(1,sizeof(pixel));
     cmd->p2 = (pixel*) calloc(1,sizeof(pixel));
     pixel *t1 = cmd->p1;
     pixel *t2 = cmd->p2;

     if ( (cmd->argc != 2 && cmd->argc != 5) || \
        ( (check_extension(args[1]))) || \
        ( (cmd->argc == 5 && !strcmp(args[cmd->argc-2],"-p") && ( check_pixel(args[3],t1) || check_pixel(args[4],t2)))))
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }
     cmd->files[0] = strdup(args[1]);
     if (cmd->argc == 5 )
          cmd->option = 10;

     return 0;
}

int parse_copy(char** args, command* cmd, int index)
{
     //copy filename.EXT
     return parse_save(args,cmd,index);
}

int parse_cut(char** args, command* cmd, int index)
{
     //cut filename.EXT [-f COLOR]
     if ( (cmd->argc != 2 && cmd->argc != 4) || \
          (check_extension(args[1])) || \
          (cmd->argc == 4 && strcmp(args[2],"-f")) )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }

     if (cmd->argc == 2)
          cmd->option = 1;
     else
     {
          cmd->option = 2;
          cmd->color = strdup(args[3]);
     }

     cmd->files[0] = strdup(args[1]);
     return 0;
}
int parse_paste(char** args, command* cmd, int index)
{
     //paste filename.EXT -p (X,Y)
     cmd->p1 = (pixel*) calloc(1,sizeof(pixel));
     pixel *t1 = cmd->p1;

     if ( cmd-> argc != 4 || check_extension(args[1]) || strcmp(args[2],"-p") || check_pixel(args[3],t1) )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }
     cmd->files[0] = strdup(args[1]);
     return 0;
}

int parse_symetric(char** args, command* cmd, int index)
{
     // symetric syntax: symetric filename.EXT -d V|H\n"
     if ( cmd-> argc != 4 || check_extension(args[1]) || strcmp(args[2],"-d") || \
          (strcasecmp(args[3],"V") && strcasecmp(args[3],"H")) )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }

     cmd->files[0] = strdup(args[1]);
     if ( !strcasecmp(args[3],"V") )
          cmd->option = 1;
     else
          cmd->option = 2;
     return 0;
}

int parse_rotate(char** args, command* cmd, int index)
{
     // rotate filename.EXT -a ANGLE\n",
     if ( cmd-> argc != 4 || check_extension(args[1]) || strcmp(args[2],"-a") || ! atoi(args[3]) )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }
     cmd->files[0] = strdup(args[1]);
     cmd->option = atoi(args[3]);
     return 0;
}

int parse_scale(char** args, command* cmd, int index)
{
     // scale filename.EXT -p (100,200) (300,400) [-c]\n",
     cmd->p1 = (pixel*) calloc(1,sizeof(pixel));
     cmd->p2 = (pixel*) calloc(1,sizeof(pixel));
     pixel *t1 = cmd->p1;
     pixel *t2 = cmd->p2;

     if ( (cmd->argc != 5 && cmd->argc != 6) ||  check_extension(args[1]) || \
          strcmp(args[2],"-p") || check_pixel(args[3],t1) || check_pixel(args[4],t2) || \
          ( cmd->argc == 6 && strcmp(args[5],"-c") ) )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }

     cmd->files[0] = strdup(args[1]);
     if ( cmd->argc == 5 )
          cmd->option = 1;
     else
          cmd->option = 2;

     return 0;
}

int parse_resize(char** args, command* cmd, int index)
{
     // resize filename.EXT -s (X,Y)
     cmd->p1 = (pixel*) calloc(1,sizeof(pixel));
     pixel *t1 = cmd->p1;

     if ( cmd-> argc != 4 || check_extension(args[1]) || strcmp(args[2],"-s") || check_pixel(args[3],t1) )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }

     cmd->files[0] = strdup(args[1]);
     return 0;
}

int parse_fill(char** args, command* cmd, int index)
{
     // fill filename.EXT -c COLOR
     if ( cmd->argc != 4 || check_extension(args[1]) || strcmp(args[2],"-c") )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }

     cmd->files[0] = strdup(args[1]);
     cmd->color = strdup(args[3]);
     return 0;

}
int parse_replace(char** args, command* cmd, int index)
{
     // replace filename.EXT COLOR1 COLOR2 [-t]
     if ( (cmd->argc != 4 && cmd->argc != 5) ||  check_extension(args[1]) || \
          ( cmd->argc == 5 && strcmp(args[4],"-t") ) )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }
     if ( cmd->argc == 4 )
          cmd->option = 1;
     else
          cmd->option = 2;

     cmd->files[0] = strdup(args[1]);
     cmd->color = strdup(args[2]);
     strcat(cmd->color,"$");
     strcat(cmd->color,args[3]);

     return 0;
}

int parse_negative(char** args, command* cmd, int index)
{
     //negative filename.EXT
     if ( cmd->argc != 2 || check_extension(args[1]) )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }

     cmd->files[0] = strdup(args[1]);
     return 0;
}

int parse_gray(char** args, command* cmd, int index)
{
     // gray filename.EXT
     return parse_negative(args,cmd,index);
}

int parse_black_white(char** args, command* cmd, int index)
{
     // black_white filename.EXT
     return parse_negative(args,cmd,index);
}

int parse_brightness(char** args, command* cmd, int index)
{
     // brightness filename.EXT
     return parse_negative(args,cmd,index);
}

int parse_contrast(char** args, command* cmd, int index)
{
     // contrast filename.EXT
     return parse_negative(args,cmd,index);
}

int parse_write_script(char** args, command* cmd, int index)
{
     // write_script filename.cimp
     if ( cmd->argc != 2 || strlen(args[1]) < 6 || strcmp(args[1]+strlen(args[1])-5, ".cimp") )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }
     cmd->files[0] = strdup(args[1]);
     return 0;
}

int parse_load_script(char** args, command* cmd, int index)
{
     // load_script filename.cimp
     return parse_write_script(args,cmd,index);
}

int parse_edit_script(char** args, command* cmd, int index)
{
     // edit_script filename.cimp
     return parse_write_script(args,cmd,index);
}

int parse_save_script(char** args, command* cmd, int index)
{
     // save_script filename.cimp
     return parse_write_script(args,cmd,index);
}

int parse_rename_script(char** args, command* cmd, int index)
{
     // rename_script filename.cimp -o filename_.cimp
     if ( cmd->argc != 4 || strlen(args[1]) < 6 || strcmp(args[1]+strlen(args[1])-5, ".cimp") ||\
          strcmp(args[2],"-o") || strlen(args[3]) < 6 || strcmp(args[1]+strlen(args[3])-5, ".cimp") )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }
     cmd->files[0] = strdup(args[1]);
     cmd->files[1] = strdup(args[3]);
     return 0;
}

int parse_execute_script(char** args, command* cmd, int index)
{
     // execute_script filename.cimp
     return parse_write_script(args,cmd,index);
}

int parse_undo(char** args, command* cmd, int index)
{
     // undo filename.EXT
     return parse_negative(args,cmd,index);
}

int parse_redo(char** args, command* cmd, int index)
{
     // redo filename.EXT
     return parse_negative(args,cmd,index);
}

int parse_modify_bpc(char** args, command* cmd, int index)
{
     // modify_bpc filename.EXT -m 1|2|3
     if ( cmd->argc != 4 || check_extension(args[1]) || strcmp(args[2],"-m") || atoi(args[3]) < 1 || atoi(args[3]) > 3 )
     {
          cmd = NULL;
          fprintf(stderr,"%s",help[index]);
          return -1;
     }

     cmd->files[0] = strdup(args[1]);
     cmd->option = atoi(args[3]);
     return 0;
}

int parse_art_effect(char** args, command* cmd, int index)
{
     // art_effect filename.EXT -m 1|2|3
     return parse_modify_pbc(args,cmd,index);
}

int main()
{
     command* cmd = (command*) calloc(1,sizeof(command));
     cmd->files = (char**) calloc(4,sizeof(char*));
     char line[SIZE];
     while(1)
     {
          printf("> ");
          fgets(line,SIZE,stdin);
          parse(line,cmd);
     }
     return 0;
}
