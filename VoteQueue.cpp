#include "VoteQueue.h"

void addToQueue(VoteQueue &voteQueue, VoteRequest request)
{
    voteQueue.requests.push(request);
}

bool queueIsEmpty(VoteQueue &voteQueue)
{
    return voteQueue.requests.empty();
}

VoteRequest takeFromQueue(VoteQueue &voteQueue)
{
    VoteRequest request = voteQueue.requests.front();
    voteQueue.requests.pop();

    return request;
}

