#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <sys/wait.h>
#include "parser.h"
#include "linker.h"

char *cmd_name[] =
{
    "open",
    "save",
    "set_bg",
    "select_rect",
    "select_free",
    "select_color",
    "unselect",
    "copy",
    "cut",
    "paste",
    "symetric",
    "rotate",
    "scale",
    "resize",
    "fill",
    "replace",
    "negative",
    "gray",
    "black_white",
    "brightness",
    "contrast",
    "blur",
    "pixel",
    "undo",
    "redo",
    "close",
    "write_script",
    "load_script",
    "edit_script",
    "rename_script",
    "execute_script",
    "cd",
    "ls",
    "batch_mode",
    "help",
    "exit"
};

int (*parse_func[]) (char **, command*) =
{
    &parse_open,
    &parse_save,
    &parse_set_bg,
    &parse_select_rect,
    &parse_select_free,
    &parse_select_color,
    &parse_unselect,
    &parse_copy,
    &parse_cut,
    &parse_paste,
    &parse_symetric,
    &parse_rotate,
    &parse_scale,
    &parse_resize,
    &parse_fill,
    &parse_replace,
    &parse_negative,
    &parse_gray,
    &parse_black_white,
    &parse_brightness,
    &parse_contrast,
    &parse_blur,
    &parse_pixel,
    &parse_undo,
    &parse_redo,
    &parse_close,
    &parse_write_script,
    &parse_load_script,
    &parse_edit_script,
    &parse_rename_script,
    &parse_execute_script,
    &parse_cd,
    &parse_ls,
    &parse_batch_mode,
    &parse_help,
    &parse_exit
};

char* syntax[] =
{
    "open filename.EXT [-o filename_.EXT]\n",
    "save filename.EXT -o filename.EXT\n",
    "set_bg filename.EXT -c color\n",
    "select_rect filename.EXT [-m ADD|SUB|OVERWRITE] [-p (X1,Y1) (X2,Y2)]\n",
    "select_free filename.EXT [-m ADD|SUB|OVERWRITE]\n",
    "select_color filename.EXT [-m ADD|SUB|OVERWRITE] -c color [-t 0-100]\n",
    "unselect filename.EXT [-p (X1,Y1) (X2,Y2)]\n",
    "copy filename.EXT\n",
    "cut filename.EXT [-f color]\n",
    "paste filename.EXT -p (X,Y)\n",
    "symetric filename.EXT -d V|H\n",
    "rotate filename.EXT -a ANGLE\n",
    "scale filename.EXT -p (100,200) (300,400) \nscale filename.EXT -m N -c color\n",
    "resize filename.EXT -s (X,Y)\n",
    "fill filename.EXT -c color\n",
    "replace filename.EXT color color [-t 0-100]\n",
    "negative filename.EXT\n",
    "gray filename.EXT\n",
    "black_white filename.EXT -t 0-100\n",
    "brightness filename.EXT -r 0-200\n",
    "contrast filename.EXT -r 0-200\n",
    "blur filename.EXT -r 0-200\n",
    "pixel filename.EXT -s PIXELSIZE\n",
    "undo filename.EXT\n",
    "redo filename.EXT\n",
    "close filename.EXT\n",
    "write_script filename.cimp\n",
    "load_script filename.cimp\n",
    "edit_script filename.cimp\n",
    "rename_script filename.cimp -o filename_.cimp\n",
    "execute_script filename.cimp\n",
    "cd [ref]\n",
    "ls [ref]\n",
    "batch_mode 0|1|2|\n",
    "help [command]\n",
    "exit [value]\n"
};

char* help[] =
{
     "open an image for editing in a new window or in an existing one\n",
     "save an image in a giving format\n",
     "set the default background color of an image\n",
     "select from an image a rectangular region for adding or substracting or overwriting using two points or by cursor\n",
     "select from an image a region using the cursor for adding or substracting or overwriting\n",
     "select from an image a region by specific a color for adding or substracting or overwriting, a toleration value can be specified\n",
     "unselect a region that has been select by giving its position by two points or by cursor\n",
     "copy from an image a region that has been selected\n",
     "cut from an image a region that has been selected, a font color can be specified\n",
     "paste to an image a region that has been copied or cut on a specific position\n",
     "make an image symetric\n",
     "rotate an image by a specific angle\n",
     "scale an image by zooming on a specific position or by adding a margin value\n",
     "resize an image to the specified dimensions\n",
     "fill an image with a specific color\n",
     "replace a color by an other one in an image, a toleration value can be specified\n",
     "make an image in negative mode\n",
     "make an image in gray mode\n",
     "make an image in black & white mode with a specific toleration value\n",
     "make an image in brightness mode with a specific rate value\n",
     "make an image in contrast mode with a specific rate value\n",
     "make an image in blur mode with a specific rate value\n",
     "make an image in pixel with a specific pixel size\n",
     "undo the last action that has been done on an image\n",
     "redo the last action that has been done on an image\n",
     "close the window where the image is loaded\n",
     "write a cimp script\n",
     "load a cimp script\n",
     "edit a cimp script\n",
     "rename a cimp script\n",
     "execute a cimp script\n",
     "change directory\n",
     "list a reference\n",
     "set the batch mode\n",
     "print the help of a command\n",
     "exit cimp\n"
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
     int i = e != NULL ? (int)(e - arg) : 0;

     if ( arg[0] != '(' || arg[end] != ')' || i <= 1 || i == end - 1 || i == end  || end < 4 )
          return -1;
     else
     {
          char s1[24];
          char s2[24];
          strncpy(s1,arg+1,i-1);
          strncpy(s2,arg+i+1,end-i+1);
          p->x = atoi(s1);
          p->y = atoi(s2);
          return 0;
     }
}

char* name_to_rgb(char* arg)
{
     char* colors[] = {"white","silver","gray","black","red","maron","yellow","olive","lime","green","aqua","teal","blue","navy","fuchisa","purple","pink"};
     char* rgbs[] ={"(255,255,255)","(192,192,192)","(128,128,128)","(0,0,0)","(255,0,0)","(128,0,0)","(255,255,0)","(128,128,0)","(0,255,0)","(0,128,0)","(0,255,255)","(0,128,128)","(0,0,255)","(0,0,128)","(255,0,255)","(128,0,128)","(255,192,203)" };
     for (int i=0; i<17; i++)
          if ( ! strcasecmp(colors[i],arg) )
          {
               arg = rgbs[i];
               return arg;
          }
     return NULL;
}

int check_color(char* arg, color* c)
{
     if ( arg[0] != '(' )
          if ( ! (arg = name_to_rgb(arg)) )
               return -1;

     char sourceCopy[strlen(arg) + 1];
     char * regexString = "\\(([0-9]{1,3}),([0-9]{1,3}),([0-9]{1,3})\\)";
     size_t maxGroups = 4;

     regex_t regexCompiled;
     regmatch_t groupArray[maxGroups];

     if (regcomp(&regexCompiled, regexString, REG_EXTENDED))
          return -1;

     if (regexec(&regexCompiled, arg, maxGroups, groupArray, 0) == 0)
     {
          for (int i=1; i<maxGroups; i++)
          {
               strcpy(sourceCopy, arg);
               sourceCopy[groupArray[i].rm_eo] = 0;

               if ( i == 1 )
                    c->r = atoi(sourceCopy + groupArray[i].rm_so);
               else if ( i == 2 )
                    c->g = atoi(sourceCopy + groupArray[i].rm_so);
               else
                    c->b = atoi(sourceCopy + groupArray[i].rm_so);
          }
     }
     regfree(&regexCompiled);
     return 0;
}

int batch_files(int argc, char** args, char** list)
{
     int j = 0;
     for (int i=1; i<argc; i++)
          if ( strchr(args[i],'.') )
               list[j++] = strdup(args[i]);
          else
               break;
     return j;
}

int batch_regex(char* reg_file, char** list)
{
     int i = 0;
     char ls_regex[SIZE];
     sprintf(ls_regex,"ls -1f %s > .ls_regex 2>&1",reg_file);
     FILE* f;
     pid_t pid = fork();

     if ( pid == 0 )
          execl("/bin/sh", "bin/sh", "-c", ls_regex, (char*) NULL);

     int status;
     waitpid(pid, &status, 0);

     if ( WIFEXITED(status) )
     {
          int exit_status = WEXITSTATUS(status);
          if ( exit_status )
          {
               remove(".ls_regex");
               return -1;
          }
          if( (f = fopen(".ls_regex", "r")) == NULL)
          {
               remove(".ls_regex");
               return -1;
          }
          while ( ( list[i] = calloc(SIZE,sizeof(char)) ) != NULL  && fscanf(f, "%s\n", list[i++]) != EOF );
          fclose(f);
          remove(".ls_regex");
          return --i;
     }
     remove(".ls_regex");
     return -1;
}

int tokenize(char* line,char** args)
{
    int i = 0;
    for (char *token = strtok(line,SEP); token != NULL; token = strtok(NULL, SEP) )
          args[i++] = strdup(token);
    return i;
}

char** make_args(int argc, int n, char* cmd_name, char*filename,char **args)
{
     char** new_args = (char**) calloc(argc-n+1,sizeof(char*));
     new_args[0] = strdup(cmd_name);
     new_args[1] = strdup(filename);
     int j = 2;
     for (int i=n+1; i<argc; i++)
          new_args[j++] = strdup(args[i]);
     return new_args;
}

void add_cmd_to_history(char *cmd,char* file)
{
     char dest[254];
     sprintf(dest,"%s/.%s_history",getenv("HOME"),file);
     FILE * fp;

     if ( ( fp = fopen(dest,"a+") ) == NULL)
          fprintf(stderr,"cimp: add_cmd_to_history(): fopen failed.\n");

     if ( ( fprintf(fp,"%s",cmd) ) == -1 )
          fprintf(stderr,"cimp: add_cmd_to_history(): fprintf failed.\n");

     if (fp)
          fclose(fp);
}

void debug(char* line, command* cmd, char** args)
{
     printf("line: %s\n",line );
     fprintf(stdout,"args:\n" );
     for (int i =0; i<cmd->argc; i++)
          fprintf(stdout,"|%s",args[i]);
     fprintf(stdout,"\n");
     fprintf(stdout,"command:\n");
     fprintf(stdout,"\t|argc: %d\n",cmd->argc);
     fprintf(stdout,"\t|index: %d\n",cmd->index);
     fprintf(stdout,"\t|value: %d\n",cmd->value);
     fprintf(stdout,"\t|option: %d\n",cmd->option);
     if ( cmd->files[0] )
          fprintf(stdout,"\t|files[0]: %s\n",cmd->files[0]);
     if ( cmd->files[1] )
          fprintf(stdout,"\t|files[1]: %s\n",cmd->files[1]);
     if ( cmd->pixels[0] )
          fprintf(stdout,"\t|pixels[0]: %d %d\n",cmd->pixels[0]->x,cmd->pixels[0]->y);
     if ( cmd->pixels[1] )
          fprintf(stdout,"\t|pixels[1]: %d %d\n",cmd->pixels[1]->x,cmd->pixels[1]->y);
     if ( cmd->colors[0] )
          fprintf(stdout,"\t|colors[0]: %d %d %d\n",cmd->colors[0]->r,cmd->colors[0]->g,cmd->colors[0]->b);
     if ( cmd->colors[1] )
          fprintf(stdout,"\t|colors[1]: %d %d %d\n",cmd->colors[1]->r,cmd->colors[0]->g,cmd->colors[0]->b);
}

int parse_by_mode (char* line, char** args, command* cmd)
{
     cmd->argc = tokenize(line,args);
     cmd->index = get_index(args[0]);
     if ( cmd->index == -1 )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          fprintf(stderr,"cimp_parser(): command unknwon\n");
          return -1;
     }
     if ( cmd->index > 22 )
          return parse_func[cmd->index](args,cmd);

     if ( BATCH_MODE == 0 )
     {
          if ( ! parse_func[cmd->index](args,cmd) )
          {
               if ( !call_command(cmd) )
                    add_cmd_to_history(line,cmd->files[0]);                                       // ajout de la commande à l'historique
          }
     }
     else if ( BATCH_MODE == 1 )
     {
          char** list = calloc(SIZE,sizeof(char*));
          int n = batch_regex(args[1],list);
          for (int i=0; i<n; i++)
          {
               args[1] = strdup(list[i]);
               if ( ! parse_func[cmd->index](args,cmd) )
               {
                    if ( !call_command(cmd) )
                         add_cmd_to_history(line,cmd->files[0]);                                       // ajout de la commande à l'historique
               }
          }
     }
     else
     {
          char** list = calloc(SIZE,sizeof(char*));
          int n = batch_files(cmd->argc,args,list);
          cmd->argc = cmd->argc - n + 1;

          for (int i=0; i<n; i++)
          {
               char** new_args = make_args(cmd->argc, n, args[0],list[i],args);
               if ( ! parse_func[cmd->index](new_args,cmd) )
               {
                    if ( !call_command(cmd) )
                         add_cmd_to_history(line,cmd->files[0]);       // ajout de la commande à l'historique
               }
          }
     }
     return 0;
}

int get_index(char* name)
{
     int i;
     for ( i=0; i < NUM_PARSE; i++ )
     {
          if ( ! strcasecmp(name,cmd_name[i]) )
               return i;
     }
     return -1;
}

int parse_open(char** args, command* cmd)
{
     // open filename.EXT [-o filename_.EXT]

     if ( (cmd->argc != 2 && cmd->argc != 4) ||  check_extension(args[1]) || \
          (cmd->argc == 4 && ( strcmp(args[2],"-o") || check_extension(args[3]) ) ) )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }

     if (cmd->argc == 2)
     {
          cmd->option = 0;
          cmd->files[0] = strdup(args[1]);
          return 0;
     }
     else
     {
          cmd->option = 1;
          cmd->files[0] = strdup(args[1]);
          cmd->files[1] = strdup(args[3]);
          return 0;
     }
}

int parse_save(char** args, command* cmd)
{
     // save filename.EXT -o filename.EXT
     if ( BATCH_MODE && cmd->argc != 2 )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          fprintf(stderr,"cimp_parser(): %s doesn't support batch mode\n",args[0]);
          return -1;
     }

     else if ( cmd->argc != 4 || check_extension(args[1]) || check_extension(args[3]) || strcmp(args[2],"-o") )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     else
     {
          cmd->files[0] = strdup(args[1]);
          cmd->files[1] = strdup(args[3]);
          return 0;
     }
}

int parse_set_bg(char** args, command* cmd)
{
     // set_bg filename.EXT -c color

     cmd->colors[0] = (color*) calloc(1,sizeof(color));
     color *c1 = cmd->colors[0];
     if ( cmd->argc != 4 || check_extension(args[1]) || strcmp(args[2],"-c") || check_color(args[3],c1) )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     else
     {
          cmd->files[0] = strdup(args[1]);
          return 0;
     }
}

int parse_select_rect(char** args, command* cmd)
{
     //select_rect filename.EXT [-m OVERWRITE|ADD|SUB] [-p (X1,Y1) (X2,Y2)]
     cmd->pixels[0] = (pixel*) calloc(1,sizeof(pixel));
     cmd->pixels[1] = (pixel*) calloc(1,sizeof(pixel));
     pixel *t1 = cmd->pixels[0];
     pixel *t2 = cmd->pixels[1];

     if (
          ( (cmd->argc != 2) && (cmd->argc != 4) && (cmd->argc != 5) && (cmd->argc != 7) ) || \
          ( (cmd->argc >= 2) && (check_extension(args[1])) )  || \
          ( (cmd->argc >= 3) && (strcmp(args[2],"-p")) && (strcmp(args[2],"-m")) ) || \
          ( (cmd->argc == 4) && (!strcmp(args[2],"-p")) ) || \
          ( (cmd->argc == 6) && (!strcmp(args[4],"-p")) ) || \
          ( (cmd->argc >= 4) && (!strcmp(args[2],"-m")) && (strcasecmp(args[3],"OVERWRITE")) && (strcasecmp(args[3],"ADD")) && (strcasecmp(args[3],"SUB")) ) || \
          ( (cmd->argc >= 5) && (!strcmp(args[2],"-p")) && ( (check_pixel(args[3],t1)) || (check_pixel(args[4],t2))) )|| \
          ( (cmd->argc == 7) && (strcmp(args[4],"-p")) && ( (check_pixel(args[5],t1)) || (check_pixel(args[6],t2))) )
        )

     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }

     cmd->option = 0;
     cmd->files[0] = strdup(args[1]);
     if ( cmd->argc >= 4 &&  ! strcmp(args[2],"-m") )
     {
          if ( !strcasecmp(args[3],"OVERWRITE"))
               cmd->option += 0;
          else if ( !strcasecmp(args[3],"ADD"))
               cmd->option += 1;
          else
               cmd->option += 2;
     }

     if ( (cmd->argc >= 5 &&  !strcmp(args[2],"-p")) || (cmd->argc == 7 &&  !strcmp(args[4],"-p"))  )
          cmd->option += 10;

     return 0;
}

int parse_select_free(char** args, command* cmd)
{
     //select_free filename.EXT [-m OVERWRITE|ADD|SUB]
     if (
          ( (cmd->argc != 2) &&  (cmd->argc != 4) )|| \
          ( (cmd->argc >= 2) && (check_extension(args[1])) )  || \
          ( (cmd->argc == 4) && ( (strcmp(args[2],"-m")) || ( (strcasecmp(args[3],"OVERWRITE")) && (strcasecmp(args[3],"ADD")) && (strcasecmp(args[3],"SUB")) ) ) )
        )

     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }

     cmd->option = 0;
     cmd->files[0] = strdup(args[1]);
     if ( cmd->argc == 4 )
     {
          if ( !strcasecmp(args[3],"OVERWRITE"))
               cmd->option = 0;
          else if ( !strcasecmp(args[3],"ADD"))
               cmd->option = 1;
          else
               cmd->option = 2;
     }

     return 0;
}

int parse_select_color(char** args, command* cmd)
{
     //select_color filename.EXT [-m ADD|SUB|OVERWRITE] -c color [-t 0-100]
     cmd->colors[0] = (color*) calloc(1,sizeof(color));
     color *c1 = cmd->colors[0];

     if (
          ( (cmd->argc < 4 ) ||  (cmd->argc > 8) )|| \
          ( (cmd->argc >= 2) && (check_extension(args[1])) )  || \
          ( (cmd->argc >= 3) && (strcmp(args[2],"-m"))  && (strcmp(args[2],"-c")) ) || \
          ( (cmd->argc >= 4) && (!strcmp(args[2],"-m")) && (strcasecmp(args[3],"OVERWRITE")) && (strcasecmp(args[3],"ADD")) && (strcasecmp(args[3],"SUB")) ) || \
          ( (cmd->argc == 6) && (!strcmp(args[2],"-c")) && (check_color(args[3],c1) || strcmp(args[4],"-t") || atoi(args[5]) < 1  || atoi(args[5]) > 100 )  ) || \
          ( (cmd->argc == 8) && (!strcmp(args[4],"-c")) && (check_color(args[5],c1) || strcmp(args[6],"-t") || atoi(args[7]) < 1  || atoi(args[7]) > 100 )  )
        )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }

     cmd->option = 0;
     cmd->value = 0;
     cmd->files[0] = strdup(args[1]);
     if ( cmd->argc >= 4 &&  ! strcmp(args[2],"-m") )
     {
          if ( !strcasecmp(args[3],"OVERWRITE"))
               cmd->option = 0;
          else if ( !strcasecmp(args[3],"ADD"))
               cmd->option = 1;
          else
               cmd->option = 2;
     }

     if ( cmd->argc >= 4 &&  ! strcmp(args[2],"-c") )
     {
          if ( cmd->argc == 6 && ! strcmp(args[4],"-t") )
               cmd->value = atoi(args[5]);
     }

     if ( cmd->argc >= 6 &&  ! strcmp(args[4],"-c") )
     {
          if ( cmd->argc == 8 && ! strcmp(args[6],"-t") )
               cmd->value = atoi(args[7]);
     }

     return 0;
}


int parse_unselect(char** args, command* cmd)
{
     //unselect filename.EXT [-p (X1,Y1) (X2,Y2)]
     cmd->pixels[0] = (pixel*) calloc(1,sizeof(pixel));
     cmd->pixels[1] = (pixel*) calloc(1,sizeof(pixel));
     pixel *t1 = cmd->pixels[0];
     pixel *t2 = cmd->pixels[1];

     if ( (cmd->argc != 2 && cmd->argc != 5) || \
        ( (check_extension(args[1]))) || \
        ( (cmd->argc == 5) && ( strcmp(args[cmd->argc-2],"-p") || check_pixel(args[3],t1) || check_pixel(args[4],t2) ) )
        )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     cmd->files[0] = strdup(args[1]);
     if (cmd->argc == 5 )
          cmd->option = 10;

     return 0;
}

int parse_copy(char** args, command* cmd)
{
     //copy filename.EXT
     if ( cmd-> argc != 2 || check_extension(args[1]) )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     cmd->files[0] = strdup(args[1]);
     return 0;
}

int parse_cut(char** args, command* cmd)
{
     //cut filename.EXT [-f color]
     cmd->colors[0] = (color*) calloc(1,sizeof(color));
     color *c1 = cmd->colors[0];

     if ( BATCH_MODE )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          fprintf(stderr,"cimp_parser(): %s doesn't support batch mode\n",args[0]);
          return -1;
     }

     else if ( (cmd->argc != 2 && cmd->argc != 4) || \
          (check_extension(args[1])) || \
          (cmd->argc == 4 && ( strcmp(args[2],"-f") || check_color(args[3],c1)) )
             )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }

     cmd->files[0] = strdup(args[1]);
     if (cmd->argc == 2)
          cmd->option = 1;
     else
          cmd->option = 2;

     return 0;
}
int parse_paste(char** args, command* cmd)
{
     //paste filename.EXT -p (X,Y)
     cmd->pixels[0] = (pixel*) calloc(1,sizeof(pixel));
     pixel *t1 = cmd->pixels[0];

     if ( cmd-> argc != 4 || check_extension(args[1]) || strcmp(args[2],"-p") || check_pixel(args[3],t1) )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     cmd->files[0] = strdup(args[1]);
     return 0;
}

int parse_symetric(char** args, command* cmd)
{
     // symetric syntax: symetric filename.EXT -d V|H
     if ( cmd-> argc != 4 || check_extension(args[1]) || strcmp(args[2],"-d") || \
          (strcasecmp(args[3],"V") && strcasecmp(args[3],"H")) )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }

     cmd->files[0] = strdup(args[1]);
     if ( !strcasecmp(args[3],"V") )
          cmd->value = 1;
     else
          cmd->value = 2;
     return 0;
}

int parse_rotate(char** args, command* cmd)
{
     // rotate filename.EXT -a ANGLE
     if ( cmd-> argc != 4 || check_extension(args[1]) || strcmp(args[2],"-a") || ! atoi(args[3]) )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     cmd->files[0] = strdup(args[1]);
     cmd->value = atoi(args[3]);
     return 0;
}

int parse_scale(char** args, command* cmd)
{
     // scale filename.EXT  -p (100,200) (300,400) |  scale filename.EXT -m N -c color
     cmd->pixels[0] = (pixel*) calloc(1,sizeof(pixel));
     cmd->pixels[1] = (pixel*) calloc(1,sizeof(pixel));
     cmd->colors[0] = (color*) calloc(1,sizeof(color));
     pixel *t1 = cmd->pixels[0];
     pixel *t2 = cmd->pixels[1];
     color *c1 = cmd->colors[0];

     if ( ((cmd->argc != 5 && cmd->argc != 6) || check_extension(args[1])) || \
          ((cmd->argc != 5 && !strcmp(args[2],"-p")) ) || \
          ((cmd->argc != 6 && !strcmp(args[2],"-m")) ) || \
          ((strcmp(args[2],"-p")) && (strcmp(args[2],"-m")) ) || \
          ((cmd->argc == 5) && (!strcmp(args[2],"-p")) && (check_pixel(args[3],t1) || check_pixel(args[4],t2)) ) || \
          ((cmd->argc == 6) && ( strcmp(args[2],"-m") ||  atoi(args[3]) < 1 || strcmp(args[4],"-c") || check_color(args[5],c1) ) )
        )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }

     cmd->files[0] = strdup(args[1]);
     if ( cmd->argc == 5 )
          cmd->option = 1;
     else
     {
          cmd->option = 2;
          cmd->value = atoi(args[3]);
     }

     return 0;
}

int parse_resize(char** args, command* cmd)
{
     // resize filename.EXT -s (X,Y)
     cmd->pixels[0] = (pixel*) calloc(1,sizeof(pixel));
     pixel *t1 = cmd->pixels[0];

     if ( cmd-> argc != 4 || check_extension(args[1]) || strcmp(args[2],"-s") || check_pixel(args[3],t1) )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }

     cmd->files[0] = strdup(args[1]);
     return 0;
}

int parse_fill(char** args, command* cmd)
{
     // fill filename.EXT -c color
     cmd->colors[0] = (color*) calloc(1,sizeof(color));
     color *c1 = cmd->colors[0];

     if ( cmd->argc != 4 || check_extension(args[1]) || strcmp(args[2],"-c") || check_color(args[3],c1))
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }

     cmd->files[0] = strdup(args[1]);
     return 0;

}
int parse_replace(char** args, command* cmd)
{
     // replace filename.EXT color color [-t 0-100]
     cmd->colors[0] = (color*) calloc(1,sizeof(color));
     cmd->colors[1] = (color*) calloc(1,sizeof(color));
     color *c1 = cmd->colors[0];
     color *c2 = cmd->colors[0];

     if (
          (cmd->argc != 4 && cmd->argc != 5) ||  check_extension(args[1]) || \
          (check_color(args[2],c1) || check_color(args[3],c2) ) || \
          (cmd->argc == 6 && strcmp(args[4],"-t") && ( (atoi(args[5]) < 1 ) || (atoi(args[5]) > 100 ) ) )
        )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     cmd->value = 0;
     cmd->files[0] = strdup(args[1]);
     if ( cmd->argc == 6 )
          cmd->value = atoi(args[5]);

     return 0;
}

int parse_negative(char** args, command* cmd)
{
     //negative filename.EXT
     if ( cmd->argc != 2 || check_extension(args[1]) )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }

     cmd->files[0] = strdup(args[1]);
     return 0;
}

int parse_gray(char** args, command* cmd)
{
     // gray filename.EXT
     return parse_negative(args,cmd);
}

int parse_black_white(char** args, command* cmd)
{
     // black_white filename.EXT -r 0-200
     if ( cmd-> argc != 4 || check_extension(args[1]) || strcmp(args[2],"-r") || atoi(args[3]) < 1 || atoi(args[3]) > 200)
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     cmd->files[0] = strdup(args[1]);
     cmd->value = atoi(args[3]);
     return 0;
}

int parse_brightness(char** args, command* cmd)
{
     // brightness filename.EXT -r 0-200
     return parse_black_white(args,cmd);
}

int parse_contrast(char** args, command* cmd)
{
     // contrast filename.EXT -r 0-200
     return parse_black_white(args,cmd);
}

int parse_blur(char** args, command* cmd)
{
     // blue filename.EXT -r 0-200
     return parse_black_white(args,cmd);
}

int parse_pixel(char** args, command* cmd)
{
     // pixel filename.EXT -s SIZE
     if ( cmd-> argc != 4 || check_extension(args[1]) || strcmp(args[2],"-s") || atoi(args[3]) < 1 )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     cmd->files[0] = strdup(args[1]);
     cmd->value = atoi(args[3]);
     return 0;
}

int parse_write_script(char** args, command* cmd)
{
     // write_script filename.cimp
     if ( BATCH_MODE )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          fprintf(stderr,"cimp_parser(): %s doesn't support batch mode\n",args[0]);
          return -1;
     }

     else if ( cmd->argc != 2 || strlen(args[1]) < 6 || strcmp(args[1]+strlen(args[1])-5, ".cimp") )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     cmd->files[0] = strdup(args[1]);
     return 0;
}

int parse_load_script(char** args, command* cmd)
{
     // load_script filename.cimp
     return parse_write_script(args,cmd);
}

int parse_edit_script(char** args, command* cmd)
{
     // edit_script filename.cimp
     return parse_write_script(args,cmd);
}

int parse_save_script(char** args, command* cmd)
{
     // save_script filename.cimp
     return parse_write_script(args,cmd);
}

int parse_rename_script(char** args, command* cmd)
{
     // rename_script filename.cimp -o filename_.cimp

     if ( BATCH_MODE )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          fprintf(stderr,"cimp_parser(): %s doesn't support batch mode\n",args[0]);
          return -1;
     }

     else if ( cmd->argc != 4 || strlen(args[1]) < 6 || strcmp(args[1]+strlen(args[1])-5, ".cimp") ||\
          strcmp(args[2],"-o") || strlen(args[3]) < 6 || strcmp(args[1]+strlen(args[3])-5, ".cimp") )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     cmd->files[0] = strdup(args[1]);
     cmd->files[1] = strdup(args[3]);
     return 0;
}

int parse_execute_script(char** args, command* cmd)
{
     // execute_script filename.cimp
     return parse_write_script(args,cmd);
}

int parse_undo(char** args, command* cmd)
{
     // undo filename.EXT
     return parse_negative(args,cmd);
}

int parse_redo(char** args, command* cmd)
{
     // redo filename.EXT
     return parse_negative(args,cmd);
}

int parse_close(char** args, command* cmd)
{
     // close filename.EXT
     return parse_negative(args,cmd);
}

int parse_cd(char** args, command* cmd)
{
    if ( cmd->argc > 2 )
    {
         fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
         cmd = NULL ;
         return -1;
    }

    if ( cmd->argc == 1 )
    {
      chdir(strdup(getenv("HOME")));
      return 0;
    }
    else
    {
        if (chdir( args[1]) == 0)
            return 0;
        else
        {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          fprintf(stderr,"cimp: cd: %s: No such file or directory\n",args[1]);
          return -1;
        }
    }
}

int parse_ls(char** args, command* cmd)
{
     if ( cmd->argc > 2 )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }

     char ls[SIZE];
     if (cmd->argc == 1 )
          sprintf(ls,"ls");
     else
          sprintf(ls,"ls %s",args[1]);
     pid_t pid = fork();

     if ( pid == 0 )
     {
          execl("/bin/sh", "bin/sh", "-c", ls, (char*) NULL);
          return -1;
     }
     else
     {
          wait(NULL);
          return 0;
     }
}


int parse_batch_mode(char** args, command* cmd)
{
     //debug(line,cmd,args);
     if ( cmd->argc != 2 || atoi(args[1]) < 0 || atoi(args[1]) > 2 )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     BATCH_MODE = atoi(args[1]);
     return 0;
}

int parse_help(char** args, command* cmd)
{
     if ( cmd->argc > 2 )
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     if ( cmd->argc == 1 )
          for (int i=0; i<NUM_PARSE; i++)
               fprintf(stdout,"%s",syntax[i]);
     else
     {
          int i = get_index(args[1]);
          if ( i == -1 )
          {
               fprintf(stdout,"can't help with unknown command\n");
               return -1;
          }
          else
               fprintf(stdout,"syntax: %saction: %s",syntax[i],help[i]);
     }
     return 0;
}
int parse_exit(char** args, command* cmd)
{
     if (cmd->argc > 2)
     {
          fprintf(stderr,"cimp_parser(): syntax error: %s",syntax[cmd->index]);
          cmd = NULL ;
          return -1;
     }
     if (cmd->argc == 2)
          exit(atoi(args[1]));
     else
          exit(EXIT_SUCCESS);
}
