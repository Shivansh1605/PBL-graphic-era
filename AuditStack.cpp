#include <iostream>
#include <cstring>
#include "DSAWrapper.h"
using namespace std;

VoteQueue::VoteQueue()
{
    queueInit(&queueData);
}

bool VoteQueue::isEmpty()
{
    return queueIsEmpty(&queueData);
}

bool VoteQueue::isFull()
{
    return queueIsFull(&queueData);
}

void VoteQueue::enqueue(string voterID, int candidateID)
{
    if (!queueEnqueue(&queueData, voterID.c_str(), candidateID))
        cout << "Queue is full.\n";
}

bool VoteQueue::dequeue(string &voterID, int &candidateID)
{
    char id[ID_LENGTH];

    if (!queueDequeue(&queueData, id, &candidateID))
        return false;

    voterID = id;
    return true;
}

AuditStack::AuditStack()
{
    stackInit(&stackData);
}

bool AuditStack::isEmpty()
{
    return stackIsEmpty(&stackData);
}

bool AuditStack::isFull()
{
    return stackIsFull(&stackData);
}

void AuditStack::push(string message)
{
    if (!stackPush(&stackData, message.c_str()))
        cout << "Audit stack is full.\n";
}

string AuditStack::pop()
{
    char message[AUDIT_LENGTH];

    if (!stackPop(&stackData, message))
        return "";

    return string(message);
}

void AuditStack::display()
{
    stackDisplay(&stackData);
}
