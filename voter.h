#ifndef VOTER_H
#define VOTER_H

#include <string>
#include <vector>

using namespace std;

struct Voter
{
    string id;
    string name;
    string pin;
    bool hasVoted;
};

struct VoterManager
{
    vector<Voter> list;
};

void loadVoters(VoterManager &manager);
void saveVoters(VoterManager &manager);
int findVoter(VoterManager &manager, string id);
void registerVoter(VoterManager &manager);
bool authenticateVoter(VoterManager &manager, string id, string pin);
void markVoted(VoterManager &manager, int index);

#endif
