#include "main.h"
int pid =0;
extern char prompt[];
extern SLL* head;
char* cmd;
void my_handler_fun(int signum)
{   int status;
    // Shell (parent) receives signal only when pid == 0
    if (pid == 0)
    {
        if (signum == SIGINT)      // Ctrl + C
        {
            printf("\n%s", prompt);
            fflush(stdout);
        }
        else if (signum == SIGTSTP)   // Ctrl + Z
        {
            printf("\n%s", prompt);
            fflush(stdout);
        }
    }
    if(pid!=0)
    {
        insert_at_first(&head,pid,cmd);
    }
    if(signum == SIGCHLD)
    {
        waitpid(pid,&status,WNOHANG);
    }
    
}

void scan_input(char *prompt, char *input_str)
{
    int status;

    // Install signal handlers once (not inside loop)
    signal(SIGINT, my_handler_fun);
    signal(SIGTSTP, my_handler_fun);
    signal(SIGCHLD,my_handler_fun);

    while (1)
    {
        pid = 0;                   // No child running now
        printf("%s", prompt);
        fflush(stdout);

        scanf(" %[^\n]", input_str);

        // ------------------------------
        // PS1 command handling
        // ------------------------------
        if (strncmp(input_str, "PS1", 3) == 0)
        {
            int res = validate_ps1(input_str);
            if (!res)
            {
                printf("PS1: command not found\n");
                continue;
            }

            strcpy(prompt, input_str + 4);
            continue;
        }
        
        // ------------------------------
        // Command processing
        // ------------------------------
        cmd = get_command(input_str);
        int type = check_command_type(cmd);

        if (type == BUILTIN)
        {
            execute_internal_cmd(input_str);
        }
        else if (type == EXTERNAL)
        {
            pid = fork();      // Update GLOBAL pid

            if (pid == 0)
            {
                // Child → run external command
                execute_external_cmd(input_str);
                exit(1);
            }
            else if (pid > 0)
            {
                // Parent → wait correctly
                waitpid(pid, &status, WUNTRACED);
                pid = 0;
            }
        }
        else
        {
            perror("invalid command");
        }
    }
}


int validate_ps1(const char *input_str)
{
    int flag_space = 0, flag_case = 0, i = 0;
    if (input_str[3] == ' ')
    {
        return 0;
    }

    if (strcmp(input_str, "PS1=") == 0)
        return 0;

    return 1;
}