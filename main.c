#include "main.h"
char* external_cmd[200];
char prompt[] = "minishell";
int main()
{
    system("clear");
    extract_external_commands(external_cmd);
    
    char input_str[50];
    //scanf("%[^\n]",input_str);
    scan_input(prompt,input_str);
    /*char* cmd = get_command(input_str);
    int type = check_command_type(cmd);
    if(type == BUILTIN)
    {
        execute_internal_cmd(input_str);
    }
    else if(type == EXTERNAL)
    {
        int pid = fork();
        if(pid == 0)
        {
            execute_external_cmd(input_str);
        }
    }
    else
    {
        perror("invalid command\n");
        return 0;
    }*/
}

