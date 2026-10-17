#ifndef ADMIN_H
#define ADMIN_H
#include "voter.h"
#include "candidate.h"
#include "election.h"
#include "AuditStack.h"
class Admin {
    VoterManager& voterManager; CandidateManager& candidateManager; Election& election; AuditStack& audit;
public:
    Admin(VoterManager& vm,CandidateManager& cm,Election& e,AuditStack& log);
    void login();
};
#endif
