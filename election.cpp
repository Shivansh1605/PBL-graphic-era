#include <iostream>
#include "election.h"

using namespace std;

void castVote(VoterManager &voters, CandidateManager &candidates,
              Election &election, AuditStack &audit)
{
    loadVoters(voters);
    loadCandidates(candidates);

    cout << "\nVoting in: " << audit.electionTitle << "\n";
    cout << "Election type: " << audit.electionType << "\n";
    cout << "Purpose: " << audit.electionPurpose << "\n";

    string id;
    string pin;

    cout << "Voter ID: ";
    cin >> id;

    cout << "PIN: ";
    cin >> pin;

    if (!authenticateVoter(voters, id, pin))
    {
        cout << "Authentication failed.\n";
        addAuditEvent(audit, "Failed login: " + id);
        return;
    }

    int voterIndex = findVoter(voters, id);

    if (voterIndex == -1)
    {
        cout << "Voter not found.\n";
        return;
    }

    if (voters.list[voterIndex].hasVoted)
    {
        cout << "This voter has already voted.\n";
        addAuditEvent(audit, "Duplicate vote blocked: " + id);
        return;
    }

    showCandidates(candidates);

    int candidateID;

    cout << "Enter option ID: ";

    if (!(cin >> candidateID))
    {
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "Invalid option ID.\n";
        return;
    }

    if (findCandidate(candidates, candidateID) == -1)
    {
        cout << "Option not found.\n";
        return;
    }

    VoteRequest request;
    request.voterID = id;
    request.candidateID = candidateID;

    addToQueue(election.voteQueue, request);

    while (!queueIsEmpty(election.voteQueue))
    {
        VoteRequest current = takeFromQueue(election.voteQueue);

        if (countVote(candidates, current.candidateID))
        {
            markVoted(voters, voterIndex);

            addAuditEvent(audit, "Vote recorded: " + id);

            cout << "Vote recorded successfully.\n";
        }
    }
}

void displayResults(CandidateManager &candidates, AuditStack &audit)
{
    showElectionInfo(audit);
    showResults(candidates);
}
