#include <stdio.h>
#include <string.h>
#include "DSA.h"

/* QUEUE: FIFO (First In, First Out) */

void queueInit(VoteQueueC *q)
{
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int queueIsEmpty(VoteQueueC *q)
{
    return q->count == 0;
}

int queueIsFull(VoteQueueC *q)
{
    return q->count == MAX_QUEUE;
}

int queueEnqueue(VoteQueueC *q, const char *voterID, int candidateID)
{
    if (queueIsFull(q))
        return 0;

    q->rear = (q->rear + 1) % MAX_QUEUE;
    strncpy(q->voterIDs[q->rear], voterID, ID_LENGTH - 1);
    q->voterIDs[q->rear][ID_LENGTH - 1] = '\0';
    q->candidateIDs[q->rear] = candidateID;
    q->count++;
    return 1;
}

int queueDequeue(VoteQueueC *q, char *voterID, int *candidateID)
{
    if (queueIsEmpty(q))
        return 0;

    strcpy(voterID, q->voterIDs[q->front]);
    *candidateID = q->candidateIDs[q->front];

    q->front = (q->front + 1) % MAX_QUEUE;
    q->count--;
    return 1;
}

/* STACK: LIFO (Last In, First Out) */

void stackInit(AuditStackC *s)
{
    s->top = -1;
}

int stackIsEmpty(AuditStackC *s)
{
    return s->top == -1;
}

int stackIsFull(AuditStackC *s)
{
    return s->top == MAX_STACK - 1;
}

int stackPush(AuditStackC *s, const char *message)
{
    if (stackIsFull(s))
        return 0;

    s->top++;
    strncpy(s->messages[s->top], message, AUDIT_LENGTH - 1);
    s->messages[s->top][AUDIT_LENGTH - 1] = '\0';
    return 1;
}

int stackPop(AuditStackC *s, char *message)
{
    if (stackIsEmpty(s))
        return 0;

    strcpy(message, s->messages[s->top]);
    s->top--;
    return 1;
}

void stackDisplay(AuditStackC *s)
{
    int i;

    if (stackIsEmpty(s))
    {
        printf("No audit events.\n");
        return;
    }

    printf("\n--- Audit Log (latest first) ---\n");
    for (i = s->top; i >= 0; i--)
        printf("%s\n", s->messages[i]);
}


