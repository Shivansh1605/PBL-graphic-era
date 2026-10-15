#ifndef VOTE_QUEUE_H
#define VOTE_QUEUE_H

#include <queue>
#include <string>

struct VoteRequest {
    std::string voterID;
    int candidateID;
};

class VoteQueue {
private:
    std::queue<VoteRequest> requests;

public:
    void enqueue(const VoteRequest& request);
    bool isEmpty() const;
    VoteRequest dequeue();
};

#endif
