#include <iostream>
#include "admin.h"
#include "election.h"

using namespace std;

void adminPanel(Admin &admin, CandidateManager &candidates, AuditStack &audit)
{
    admin.password = "admin123";

    string enteredPassword;

    cout << "Enter admin password: ";
    cin >> enteredPassword;

    if (enteredPassword != admin.password)
    {
        cout << "Incorrect password.\n";
        addAuditEvent(audit, "Failed admin login");
        return;
    }

    int choice = 0;

    do
    {
        cout << "\n--- Admin Panel ---\n";
        cout << "1. View election results\n";
        cout << "2. Add candidate/option\n";
        cout << "3. View audit events\n";
        cout << "4. View election purpose\n";
        cout << "5. Return to main menu\n";
        cout << "Enter choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "Invalid input.\n";
            continue;
        }

        if (choice == 1)
        {
            displayResults(candidates, audit);
        }
        else if (choice == 2)
        {
            addCandidate(candidates);
        }
        else if (choice == 3)
        {
            showAuditEvents(audit);
        }
        else if (choice == 4)
        {
            showElectionInfo(audit);
        }
        else if (choice == 5)
        {
            cout << "Returning to main menu.\n";
        }
        else
        {
            cout << "Invalid choice.\n";
        }

    } while (choice != 5);
}
