#ifndef AUDIT_STACK_H
#define AUDIT_STACK_H

#include <stack>
#include <string>

using namespace std;
struct AuditStack
{
stack<string> events;
string electionType;
string electionTitle;
    string electionPurpose;
};

void configureElection(AuditStack &audit);
void showElectionInfo(AuditStack &audit);
void addAuditEvent(AuditStack &audit, string event);
void showAuditEvents(AuditStack &audit);

#endif
