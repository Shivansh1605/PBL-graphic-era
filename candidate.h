#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
#include <vector>

using namespace std;

struct Candidate
{
    int id;
    string name;
    string label;
    int votes;
};

struct CandidateManager
{
    vector<Candidate> list;
};

void loadCandidates(CandidateManager &manager);
void saveCandidates(CandidateManager &manager);
void showCandidates(CandidateManager &manager);
int findCandidate(CandidateManager &manager, int id);
void addCandidate(CandidateManager &manager);
bool countVote(CandidateManager &manager, int id);
void showResults(CandidateManager &manager);

#endif
