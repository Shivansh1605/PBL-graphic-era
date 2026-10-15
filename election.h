#ifndef ELECTION_H
#define ELECTION_H

#include "voter.h"
#include "candidate.h"
#include "VoteQueue.h"
#include "AuditStack.h"

class Election {
private:
    VoterManager& voterManager;
    CandidateManager& candidateManager;
    VoteQueue voteQueue;
    AuditStack audit;

public:
    Election(VoterManager& vm, CandidateManager& cm);
    void castVote();
    void showResults() const;
    void showAudit() const;
};

#endif
