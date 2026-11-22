#ifndef ELECTION_H
#define ELECTION_H
#include <string>
#include "Voter.h"
#include "Candidate.h"
#include "DSAWrapper.h"
using namespace std;

class ElectionInfo
{
private:
    string type, title, purpose;
public:
    void setup();
    void show();
};

class Election
{
public:
    void castVote(VoterManager &voters, CandidateManager &candidates,
                  VoteQueue &queue, AuditStack &audit);
};
#endif
