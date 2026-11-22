#ifndef DSA_C_MODULE_H
#define DSA_C_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_QUEUE 100
#define ID_LENGTH 50
#define AUDIT_LENGTH 200
#define MAX_STACK 100

typedef struct
{
    char voterIDs[MAX_QUEUE][ID_LENGTH];
    int candidateIDs[MAX_QUEUE];
    int front;
    int rear;
    int count;
} VoteQueueC;

typedef struct
{
    char messages[MAX_STACK][AUDIT_LENGTH];
    int top;
} AuditStackC;

/* Queue functions implemented in C */
void queueInit(VoteQueueC *q);
int queueIsEmpty(VoteQueueC *q);
int queueIsFull(VoteQueueC *q);
int queueEnqueue(VoteQueueC *q, const char *voterID, int candidateID);
int queueDequeue(VoteQueueC *q, char *voterID, int *candidateID);

/* Stack functions implemented in C */
void stackInit(AuditStackC *s);
int stackIsEmpty(AuditStackC *s);
int stackIsFull(AuditStackC *s);
int stackPush(AuditStackC *s, const char *message);
int stackPop(AuditStackC *s, char *message);
void stackDisplay(AuditStackC *s);

#ifdef __cplusplus
}
#endif

#endif
