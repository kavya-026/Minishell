#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
SLL* head;

int insert_at_first(SLL **head, int pid, const char *cmd)
{
    SLL *new = malloc(sizeof(SLL));
    if (new == NULL)
        return -1;

    new->pid = pid;
    strncpy(new->str, cmd, sizeof(new->str) - 1);
    new->str[sizeof(new->str) - 1] = '\0'; // safety

    new->next = *head;
    *head = new;

    return 0;
}
void print_list(SLL *head)
{
    if (head == NULL)
    {
        printf("List is empty\n");
        return;
    }

    printf("PID\tCOMMAND\n");
    printf("-----------------\n");

    while (head != NULL)
    {
        printf("%d\t%s\n", head->pid, head->str);
        head = head->next;
    }
}
int delete_first(SLL **head)
{
    if (*head == NULL)
    {
        return -1;   // list is empty
    }

    SLL *temp = *head;     // store current head
    *head = temp->next;    // move head to next node
    free(temp);            // free old head

    return 0;              // success
}
