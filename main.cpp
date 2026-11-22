#include <iostream>
#include "Voter.h"
#include "Candidate.h"
#include "Election.h"
#include "Admin.h"
#include "DSAWrapper.h"
using namespace std;
int main()
{
    ElectionInfo electionInfo;
    electionInfo.setup();

    VoterManager voters;
    CandidateManager candidates;
    VoteQueue queue;
    AuditStack audit;
    Admin admin;
    Election election;

    int choice;

    do
    {
        cout << "\n===== VoteSecure =====\n";
        electionInfo.show();

        cout << "\n1. Register voter\n";
        cout << "2. Cast vote\n";
        cout << "3. Show candidates/options\n";
        cout << "4. Admin panel\n";
        cout << "5. Show audit log\n";
        cout << "6. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
            voters.registerVoter();
        else if (choice == 2)
            election.castVote(voters, candidates, queue, audit);
        else if (choice == 3)
            candidates.showCandidates();
        else if (choice == 4)
            admin.panel(candidates);
        else if (choice == 5)
            audit.display();
        else if (choice == 6)
            cout << "Goodbye!\n";
        else
            cout << "Invalid choice.\n";

    } while (choice != 6);

    return 0;
}
