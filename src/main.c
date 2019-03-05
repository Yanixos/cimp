#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"

char* cimp_prompt();
char *replace_str(char *,char* ,char* );
void add_cmd_to_history(char *);
int cimp_execute(char** );
int initialize_readline();
char ** fileman_completion (const char *, int , int );
char *command_generator (const char *, int );

/*
//  liste des noms des commandes
char *cmd_name[] =                                                                
{
    "load",
    "transfer",
    "read",
    "save",
    "export",
    "select",
    "show_selected",
    "delete_selected",
    "add_selected",
    "unselect",
    "copy",
    "cut",
    "paste",
    "symetric",
    "rotate",
    "zoom",
    "size",
    "fill",
    "replace",
    "negative",
    "gray",
    "black_white",
    "brightness",
    "contrast",
    "write_script",
    "load_script",
    "edit_script",
    "save_script",
    "rename_script",
    "execute_script",
    "undo",
    "redo",
    "transparency",
    "modify_pbc",
    "angle_rotate",
    "art_effect"
}

// les des fonctions des commandes
int (*cmd_func[]) (char **) =                                                    
{
    &cimp_load,
    &cimp_transfer,
    &cimp_read,
    &cimp_save,
    &cimp_export,
    &cimp_select,
    &cimp_show_selected,
    &cimp_delete_selected,
    &cimp_add_selected,
    &cimp_unselect,
    &cimp_copy,
    &cimp_cut,
    &cimp_paste,
    &cimp_symetric,
    &cimp_rotate,
    &cimp_zoom,
    &cimp_size,
    &cimp_fill,
    &cimp_replace,
    &cimp_negative,
    &cimp_gray,
    &cimp_black_white,
    &cimp_brightness,
    &cimp_contrast,
    &cimp_write_script,
    &cimp_load_script,
    &cimp_edit_script,
    &cimp_save_script,
    &cimp_rename_script,
    &cimp_execute_script,
    &cimp_undo,
    &cimp_redo,
    &cimp_transparency,
    &cimp_modify_pbc,
    &cimp_angle_rotate,
    &cimp_art_effect,    
};
*/
int main(int argc, char **argv)
{
    char* prompt;
    char** args = (char**) calloc( SIZE , sizeof(char*) );    
    command* cmd = NULL;
    
    initialize_readline ();                                                     // initialiser la completion automatique
	
	do
    {
        prompt = cimp_prompt();             
        line = readline (prompt);                                               // lecture du commande
        
        if ( ! line  )                                                          // si il s'agit d'un CNTRL+D
            break;                                                              // on sort de la boucle

        /*
        if ( parse_command (line,cmd) )
        {
            if ( cimp_execute (cmd) )
                add_to_history(line);                                            // ajout de la commande à l'historique
        }
        */   
    } while (1);
    
    free(cmd);
    //free(args);
    fprintf(stdout,"\n");
    
    exit(EXIT_FAILURE);
}

char* cimp_prompt()
{
    char *orig = strdup(getenv("HOME"));
    char prompt[SIZE];
    char cwd[254];
    char *p;

    getcwd(cwd, sizeof(cwd));
    sprintf(prompt,"%s%s%s%s",strdup(getenv("USER")),"@cimp:",cwd,"$ ");
    
    if ( ( p = strstr(prompt, orig) ) != NULL )
        return replace_str(prompt,orig,p);
    else
        return strdup(prompt);
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


void add_cmd_to_history(char *cmd)
{
    char* cimp_history = strdup(getenv("HOME"));
    strcat(cimp_history,"/.cimp_history");
    FILE * fp;
    
    if ( ( fp = fopen(cimp_history,"a+") ) == NULL)                              
        fprintf(stderr,"cimp: add_cmd_to_history(): fopen failed.\n");

    if ( ( fprintf(fp,"%s",cmd) ) == -1 )                                   
        fprintf(stderr,"cimp: add_cmd_to_history(): fprintf failed.\n");
    
    if (fp)
        fclose(fp);
}
/*
int cimp_execute(char **args)
{
    // chercher la commande dans les commandes internes
    for (int i = 0; i < CMD_NUM; i++)                                           
            if (strcmp(args[0], cmd_name[i]) == 0) 
                return (*cmd_func[i])(args);
}
*/
int initialize_readline()
{
    rl_attempted_completion_function = fileman_completion;
    return 0;
}

char ** fileman_completion (const char *com, int start, int end)
{
    char **matches;
    matches = (char **)NULL; 
    
    if (start == 0)
        matches = rl_completion_matches (com, command_generator);
        
    return (matches);
}

char *command_generator (const char *com, int num)
{
/*
    static int indice, len;
    char *completion;

    if (num == 0)
    {
        indice = 0;
        len = strlen(com);
    }

    while (indice < NUM_CMD)
    {
        completion = cmd_funct[indice++];

        if (strncmp (completion, com, len) == 0)
            return strdup(completion);
    }
*/
    return NULL;
}
