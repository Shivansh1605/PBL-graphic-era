#ifndef CANDIDATE_H
#define CANDIDATE_H
#include <string>
#include <vector>
struct Candidate { int candidateID; std::string name, party; int votes; };
class CandidateManager {
    std::vector<Candidate> candidates;
    const std::string fileName="candidates.txt";
    void loadCandidates(); void saveCandidates() const;
public:
    CandidateManager();
    void showCandidates() const;
    bool addCandidate();
    Candidate* findCandidate(int id);
    bool addVote(int id);
    void showResults() const;
};
#endif
