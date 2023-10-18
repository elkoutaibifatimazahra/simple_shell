#ifndef _CUSTOM_SHELL_H_
#define _CUSTOM_SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

/* Buffer size for reading/writing */
#define INPUT_BUF_SIZE 1024
#define OUTPUT_BUF_SIZE 1024
#define CLEAR_BUFFER -1

/* Command chaining indicators */
#define CMD_STANDARD 0
#define CMD_CONDITION_OR 1
#define CMD_CONDITION_AND 2
#define CMD_SEQUENTIAL 3

/* For the number conversion function */
#define CONVERT_TO_LOWER 1
#define CONVERT_TO_UNSIGNED 2

/* Use the system's getline function */
#define SYS_GETLINE 0
#define SYS_STRTOK 0

#define HISTORY_FILE ".custom_shell_log"
#define MAX_HISTORY 4096

extern char **environ;

/**
 * struct linkedlist_str - single-linked list structure
 * @value: integer value
 * @text: string value
 * @nxt: points to the subsequent node
 */
typedef struct linkedlist_str
{
    int value;
    char *text;
    struct linkedlist_str *nxt;
} lst_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@input: a string generated from getline containing arguements
 *@args: an array of strings generated from arg
 *@ex_path: a string path for the current command
 *@arg_c: the argument count
 *@line_num: the error count
 *@exit_code: the error code for exit()s
 *@countline_flag: if on count this line of input
 *@prog_name: the program filename
 *@local_env: linked list local copy of environ
 *@custom_environ: custom modified copy of environ from LL env
 *@cmd_history: the history node
 *@alias_list: the alias node
 *@env_modified: on if environ was changed
 *@prev_cmd_status: the return status of the last exec'd command
 *@command_chain: address of pointer to cmd_buf, on if chaining
 *@chain_type: CMD_type ||, &&, ;
 *@input_fd: the fd from which to read line input
 *@history_line_count: the history line number count
 */

typedef struct passer
{
    char *input;
    char **args;
    char *ex_path;
    int arg_c;
    unsigned int line_num;
    int exit_code;
    int countline_flag;
    char *prog_name;
    lst_t *local_env;
    lst_t *cmd_history;
    lst_t *alias_list;
    char **custom_environ;
    int env_modified;
    int prev_cmd_status;

    char **command_chain; 
    int chain_type; 
    int input_fd;
    int history_line_count;
} pass_t;

#define DATA_DEFAULT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
        0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@tp: the builtin command flag
 *@fnc: the function
 */
typedef struct bltn
{
    char *tp;
    int (*fnc)(pass_t *);
} bltn_t;


/* hsh.c */
int shh(pass_t *, char **);
int check_bltn(pass_t *);
void fnd_cmd(pass_t *);
void frk_cmd(pass_t *);

/* path.c */
int is_a_cmd(pass_t *, char *);
char *dp_chars(char *, int, int);
char *fnd_pth(pass_t *, char *, char *);

/* loophsh.c */
int loopshh(char **);

/* err_string_functions.c */
void _epts(char *);
int _eptchr(char);
int _ptfd(char c, int fd);
int _ptsfd(char *str, int fd);

/* string_functions.c */
int strlength(char *);
int strcompare(char *, char *);
char *strts_wth(const char *, const char *);
char *strcombine(char *, char *);

/* string_functions2.c */
char *str_copy(char *, char *);
char *dup_str(const char *);
void pts(char *);
int pt_char(char);

/* string_functions3.c */
char *str_copy_n(char *, char *, int);
char *str_combine_n(char *, char *, int);
char *str_char(char *, char);

/* string_functions4.c */
char **str_to_words(char *, char *);
char **str_to_words2(char *, char );

/* memory_functions.c */
char *set_mem(char *, char, unsigned int);
void free_mem(char **);
void *realloc_mem(void *, unsigned int, unsigned int);

/* memory_functions2.c */
int buffree(void **);

/* more_functions.c */
int intrctv_mode(pass_t *);
int is_dlm(char, char *);
int is_alphbtc(int);
int str_to_int(char *);

/* more_functions2.c */
int error_str_to_int(char *);
void prnt_error(pass_t *, char *);
int prnt_integer(int, int);
char *num_cnvrt(long int, int, int);
void strip_comments(char *);

/* builtin_emulators.c */
int cmd_exit(pass_t *);
int cmd_cd(pass_t *);
int cmd_help(pass_t *);

/* builtin_emulators2.c */
int cmd_histr(pass_t *);
int cmd_alias(pass_t *);
int unset_alias(pass_t *info, char *str);
int set_alias(pass_t *info, char *str);

/* getline.c */
ssize_t gt_input(pass_t *);
int _gtline(pass_t *, char **, size_t *);
void handle_sigint(int);

/* info.c */
void reset_info(pass_t *);
void confg_info(pass_t *, char **);
void freeinfo(pass_t *, int);

/* env.c */
char *gt_env(pass_t *, const char *);
int cmd_env(pass_t *);
int cmd_setenv(pass_t *);
int cmd_unsetenv(pass_t *);
int intlz_env_list(pass_t *);

/* env2.c */
char **gt_environ(pass_t *);
int rm_env(pass_t *, char *);
int add_env(pass_t *, char *, char *);

/* file_io_functions.c */
char *access_history_file(pass_t *data);
int wr_history(pass_t *data);
int rd_history(pass_t *data);
int bld_history_list(pass_t *data, char *buffer, int line_num);
int history_renumber(pass_t *data);

/* liststr.c */
lst_t *prepend_node(lst_t **, const char *, int);
lst_t *append_node(lst_t **, const char *, int);
size_t prnt_list_str(const lst_t *);
int rm_node_index(lst_t **, unsigned int);
void frlist(lst_t **);

/* liststr2.c module */
size_t list_length(const lst_t *);
char **listtostr(lst_t *);
size_t show_list(const lst_t *);
lst_t *srch_node_start(lst_t *, char *, char);
ssize_t gt_node_index(lst_t *, lst_t *);

/* chain.c */
int is_cmd_chain(pass_t *, char *, size_t *);
void chck_chain(pass_t *, char *, size_t *, size_t, size_t);
int rplc_alias(pass_t *);
int rplc_vars(pass_t *);
int rplc_str(char **, char *);

#endif

