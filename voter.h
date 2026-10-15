#ifndef VOTER_H
#define VOTER_H

#include <string>
#include <vector>

struct Voter {
    std::string voterID;
    std::string name;
    std::string pin;
    bool hasVoted;
};

class VoterManager {
private:
    std::vector<Voter> voters;
    const std::string fileName = "voters.txt";
    void loadVoters();
    void saveVoters() const;

public:
    VoterManager();
    bool registerVoter();
    bool authenticate(const std::string& id, const std::string& pin);
    Voter* findVoter(const std::string& id);
    const std::vector<Voter>& getVoters() const;
    void markVoted(const std::string& id);
};

#endif
