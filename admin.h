#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include "candidate.h"
#include "AuditStack.h"

using namespace std;

struct Admin
{
    string password;
};

void adminPanel(Admin &admin, CandidateManager &candidates, AuditStack &audit);

#endif
