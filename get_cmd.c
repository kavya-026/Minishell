#include "main.h"

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
                    "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                    "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};


extern char* external_cmd[200];
void extract_external_commands(char **external_commands)
{
    FILE *fp = fopen("external.txt", "r");
    if (!fp)
    {
        perror("cannot open external_commands.txt");
        return;
    }

    char command[100];
    int j = 0;

    while (fgets(command, sizeof(command), fp))
    {
        command[strcspn(command, "\n")] = '\0';  // remove newline

        external_commands[j] = malloc(strlen(command) + 1);
        strcpy(external_commands[j], command);
        j++;
    }

    external_commands[j] = NULL;
    fclose(fp);
}

char *get_command(char *input_str)
{
    char *got_cmd;
    int i = 0, j = 0;
    while (input_str[i] == ' ') // remove leading space
        i++;
    got_cmd = malloc(strlen(input_str) + 1);
    while (input_str[i] != ' ' && input_str[i] != '\0')
    {
        got_cmd[j++] = input_str[i++];
    }
    got_cmd[j] = '\0';
    return got_cmd;
}

int check_command_type(char *command)
{
    int i = 0;
    while (builtins[i] != NULL)
    {
        if (strcmp(command, builtins[i]) == 0)
            return BUILTIN;

        i++;
    }
    i = 0;
    while (external_cmd[i] != NULL)
    {
        if (strcmp(command, external_cmd[i]) == 0)
        {
            return EXTERNAL;
        }
        i++;
    }
    return NO_COMMAND;
}