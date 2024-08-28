#ifndef _SHELL_H_
#define _SHELL_H_

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

/* Read/Write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* Command Chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* Convert Numbers() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* System getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* shell_control.c */
int shell_control(info_t *, char **);
int builtin_finder(info_t *);
void find_command(info_t *);
void fork_command(info_t *);

/* exec_check.c */
int is_cmd(info_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* errors1.c */
void err_puts(char *);
int err_putchar(char);
int fd_putchar(char c, int fd);
int fd_putchar_str(char *str, int fd);

/* string_functions1.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* string_functions2.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* shell_exit.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* shell_tokenizer.c */
char **strtow1(char *, char *);
char **strtow2(char *, char);

/* memory_alloc.c */
char *_memset(char *, char, unsigned int);
void sfree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* free_ptr.c.c */
int bfree(void **);

/* shell_utils.c */
int interactive(info_t *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* errors2.c */
int err_atoi(char *);
void print_err(info_t *, char *);
int print_dec(int, int);
char *convert_num(long int, int, int);
void delete_comments(char *);

/* builtin1.c */
int my_exit(info_t *);
int my_cd(info_t *);
int my_help(info_t *);

/* builtin2.c */
int my_history(info_t *);
int my_alias(info_t *);

/* get_line.c */
ssize_t get_input(info_t *);
int _getline(info_t *, char **, size_t *);
void sigintHandler(int);

/* get_info.c */
void clr_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);

/* shell_environ.c */
char *get_env(info_t *, const char *);
int my_env(info_t *);
int my_setenv(info_t *);
int my_delete_env(info_t *);
int populate_env(info_t *);

/* get_environ.c */
char **get_environ(info_t *);
int delete_env(info_t *, char *);
int _setenv(info_t *, char *, char *);

/* shell_history.c */
char *get_history(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);
int build_history(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);

/* list_functions1.c */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_str_list(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_node_list(list_t **);

/* list_functions2.c */
size_t list_len(const list_t *);
char **list_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starter(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* variable_handlers.c */
int is_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_vars(info_t *);
int replace_string(char **, char *);

#endif
