#ifndef DSA_WRAPPER_H
#define DSA_WRAPPER_H

#include <string>
#include "DSA.h"
using namespace std;

/* These C++ wrapper classes let the OOP modules use the C DSA functions. */

class VoteQueue
{
private:
    VoteQueueC queueData;

public:
    VoteQueue();
    bool isEmpty();
    bool isFull();
    void enqueue(string voterID, int candidateID);
    bool dequeue(string &voterID, int &candidateID);
};

class AuditStack
{
private:
    AuditStackC stackData;

public:
    AuditStack();
    bool isEmpty();
    bool isFull();
    void push(string message);
    string pop();
    void display();
};

#endif
