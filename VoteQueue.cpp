#include "VoteQueue.h"

void VoteQueue::enqueue(const VoteRequest& request) {
    requests.push(request);
}

bool VoteQueue::isEmpty() const {
    return requests.empty();
}

VoteRequest VoteQueue::dequeue() {
    VoteRequest request = requests.front();
    requests.pop();
    return request;
}
