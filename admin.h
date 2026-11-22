#ifndef ADMIN_H
#define ADMIN_H
#include "Candidate.h"
class Admin
{
private:
    string password;
public:
    Admin();
    void panel(CandidateManager &candidates);
};
#endif
