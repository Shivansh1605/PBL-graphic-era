#include <iostream>
#include "voter.h"
#include "candidate.h"
#include "election.h"
#include "admin.h"
#include "AuditStack.h"

using namespace std;

int main()
{
    AuditStack audit;
    configureElection(audit);

    VoterManager voters;
    CandidateManager candidates;
    Election election;
    Admin admin;

    int choice = 0;

    do
    {
        cout << "\n========== VOTESECURE ==========\n";
        showElectionInfo(audit);

        cout << "\n1. Register voter\n";
        cout << "2. Cast vote\n";
        cout << "3. View candidates/options\n";
        cout << "4. Admin panel\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1)
        {
            registerVoter(voters);
        }
        else if (choice == 2)
        {
            castVote(voters, candidates, election, audit);
        }
        else if (choice == 3)
        {
            showCandidates(candidates);
        }
        else if (choice == 4)
        {
            adminPanel(admin, candidates, audit);
        }
        else if (choice == 5)
        {
            cout << "Thank you for using VoteSecure.\n";
        }
        else
        {
            cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 5);

    return 0;
}
