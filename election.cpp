#include "election.h"
#include <iostream>

using namespace std;

Election::Election(VoterManager& vm, CandidateManager& cm)
    : voterManager(vm), candidateManager(cm) {}

void Election::castVote() {
    string id, pin;
    cout << "\n--- Voter Authentication ---\n";
    cout << "Voter ID: ";
    cin >> id;
    cout << "PIN: ";
    cin >> pin;

    if (!voterManager.authenticate(id, pin)) {
        cout << "Authentication failed. Check your ID and PIN.\n";
        audit.addEvent("Failed login attempt for ID: " + id);
        return;
    }

    Voter* voter = voterManager.findVoter(id);
    if (voter == nullptr) {
        cout << "Voter record not found.\n";
        return;
    }

    if (voter->hasVoted) {
        cout << "Vote denied: this voter has already voted.\n";
        audit.addEvent("Duplicate vote blocked for ID: " + id);
        return;
    }

    candidateManager.showCandidates();
    int candidateID;
    cout << "Enter candidate ID to vote for: ";
    if (!(cin >> candidateID)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid candidate ID.\n";
        return;
    }

    if (candidateManager.findCandidate(candidateID) == nullptr) {
        cout << "Candidate not found. Vote was not recorded.\n";
        return;
    }

    voteQueue.enqueue({id, candidateID});

    while (!voteQueue.isEmpty()) {
        VoteRequest request = voteQueue.dequeue();
        if (candidateManager.addVote(request.candidateID)) {
            voterManager.markVoted(request.voterID);
            audit.addEvent("Vote recorded for voter ID: " + request.voterID);
            cout << "Vote recorded successfully. Thank you!\n";
        } else {
            cout << "Vote could not be recorded.\n";
        }
    }
}

void Election::showResults() const {
    candidateManager.showResults();
}

void Election::showAudit() const {
    audit.showLatestEvents();
}
