#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
typedef struct node
{
    int pid;
    char str[20];
    struct node *next;
} SLL;


int insert_at_first(SLL **head, int pid, const char *cmd);
void print_list(SLL *head);
int delete_first(SLL **head);
void scan_input(char *prompt, char *input_string);
int validate_ps1(const char* input_str);
void extract_external_commands(char **external_commands);
char *get_command(char *input_str);
int check_command_type(char *command);
void execute_internal_cmd(char *input_str);
void execute_external_cmd(char *input_str);
char **convert_to_args(char *input_str);
void trim(char *str);
/*

void copy_change(char *prompt, char *input_string);


void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void signal_handler(int sig_num);
*/

#endif