#include "main.h"
extern SLL* head;
extern int pid;
void execute_internal_cmd(char *input_str)
{   SLL*  ptr;
    int i = 0;
    while (input_str[i] == ' ') // remove leading spaces
        i++;

    // pwd
    if (strcmp(input_str + i, "pwd") == 0)
    {
        char buffer[100];
        getcwd(buffer, 100);
        printf("%s\n", buffer);
    }

    // exit
    else if (strncmp(input_str + i, "exit", 4) == 0)
    {
        exit(0);
    }

    // cd
    else if (strncmp(input_str + i, "cd", 2) == 0)
    {
        // skip "cd"
        i = i + 2;

        // skip any spaces after cd
        while (input_str[i] == ' ')
            i++;

        // if no directory is given → do nothing or stay
        if (input_str[i] == '\0')
        {
            printf("No directory given\n");
            return;
        }

        // change directory
        chdir(input_str + i);

        char buffer[100];
        getcwd(buffer, 100);
        printf("%s\n", buffer);
    }
    else if (strcmp(input_str + i, "echo $") == 0)
    {
        int status;
        if (WIFEXITED(status))
        {
            WEXITSTATUS(status);
        }
    }
    else if (strcmp(input_str + i, "echo $$") == 0)
    {
        printf("%d\n", getpid());
    }
    else if (strcmp(input_str + i, "echo $SHELL") == 0)
    {
        char *shell = getenv("SHELL");
        if (shell)
            printf("%s\n", shell);
    }
    else if(strcmp(input_str + i,"jobs")==0)
    {
        print_list(head);
    }
    else if(strcmp(input_str+i,"fg")==0)
    {   int status;
        extern int pid;
        kill(ptr->pid,SIGCONT);
        waitpid(pid,&status,WUNTRACED);
        delete_first(&head);
    }
    else if(strcmp(input_str+i,"bg")==0)
    {   int status;
        kill(ptr->pid,SIGCONT);
        waitpid(pid,&status,WNOHANG);
        delete_first(&head);
    }
}
void execute_external_cmd(char *input_str)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);

    // ---------- 1. CHECK IF PIPES ARE PRESENT ----------
    if (strchr(input_str, '|') != NULL)
    {
        char left_cmd[100], right_cmd[100];

        char *pipe_pos = strchr(input_str, '|');

        strncpy(left_cmd, input_str, pipe_pos - input_str);
        left_cmd[pipe_pos - input_str] = '\0';

        strcpy(right_cmd, pipe_pos + 1);

        trim(left_cmd);
        trim(right_cmd);

        char temp1[100], temp2[100];
        strcpy(temp1, left_cmd);
        strcpy(temp2, right_cmd);

        char **left_args = convert_to_args(temp1);
        char **right_args = convert_to_args(temp2);

        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(1);
        }

        int pid1 = fork();
        if (pid1 == 0)
        {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            execvp(left_args[0], left_args);
            perror("execvp left");
            exit(1);
        }

        int pid2 = fork();
        if (pid2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            execvp(right_args[0], right_args);
            perror("execvp right");
            exit(1);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);

        return;
    }

    // ---------- NO PIPE → NORMAL EXECUTION ----------
    char input_copy[100];
    strcpy(input_copy, input_str);

    char **args = convert_to_args(input_copy);

    execvp(args[0], args);
    perror("execvp");
}
char **convert_to_args(char *input_str)
{
    char **args = malloc(20 * sizeof(char *)); // max 20 tokens
    int i = 0;

    char *token = strtok(input_str, " ");
    while (token != NULL)
    {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }

    args[i] = NULL; // last element must be NULL for execvp
    return args;
}

void trim(char *str)
{
    // Remove leading spaces
    int start = 0;
    while (str[start] == ' ')
        start++;

    // Shift string left
    if (start > 0)
        memmove(str, str + start, strlen(str + start) + 1);

    // Remove trailing spaces
    int end = strlen(str) - 1;
    while (end >= 0 && str[end] == ' ')
        str[end--] = '\0';
}
