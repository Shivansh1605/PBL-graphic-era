#ifndef VOTER_H
#define VOTER_H
#include <string>
using namespace std;

class VoterManager
{
private:
    string ids[100], names[100], pins[100];
    bool voted[100];
    int count;

public:
    VoterManager();
    void load();
    void save();
    int findVoter(string id);
    void registerVoter();
    bool checkPIN(string id, string pin);
    bool hasVoted(string id);
    void markVoted(string id);
};
#endif
