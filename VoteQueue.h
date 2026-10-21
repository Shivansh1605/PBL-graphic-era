#ifndef VOTE_QUEUE_H
#define VOTE_QUEUE_H

#include <queue>
#include <string>

using namespace std;

struct VoteRequest
{
    string voterID;
    int candidateID;
};

struct VoteQueue
{
    queue<VoteRequest> requests;
};

void addToQueue(VoteQueue &voteQueue, VoteRequest request);
bool queueIsEmpty(VoteQueue &voteQueue);
VoteRequest takeFromQueue(VoteQueue &voteQueue);

#endif
