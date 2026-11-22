#ifndef CANDIDATE_H
#define CANDIDATE_H
#include <string>
using namespace std;

class CandidateManager
{
private:
    int ids[100], votes[100], count;
    string names[100];

public:
    CandidateManager();
    void load();
    void save();
    int findCandidate(int id);
    void showCandidates();
    void addCandidate();
    bool addVote(int id);
    void results();
};
#endif
