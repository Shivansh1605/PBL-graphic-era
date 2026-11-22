#include <iostream>
#include "Admin.h"
using namespace std;

Admin::Admin()
{
    password = "admin123";
}

void Admin::panel(CandidateManager &candidates)
{
    string entered;
    cout << "Admin password: ";
    cin >> entered;

    if (entered != password)
    {
        cout << "Incorrect password.\n";
        return;
    }

    int choice;
    do
    {
        cout << "\n1. View results\n";
        cout << "2. Add candidate/option\n";
        cout << "3. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
            candidates.results();
        else if (choice == 2)
            candidates.addCandidate();

    } while (choice != 3);
}
