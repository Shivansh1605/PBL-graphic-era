#include <iostream>
#include <fstream>
#include <limits>
#include "AuditStack.h"

using namespace std;

bool loadElectionConfig(AuditStack &audit)
{
    ifstream file("election_config.txt");

    if (!getline(file, audit.electionType))
    {
        return false;
    }

    if (!getline(file, audit.electionTitle))
    {
        return false;
    }

    if (!getline(file, audit.electionPurpose))
    {
        return false;
    }

    return true;
}

void saveElectionConfig(AuditStack &audit)
{
    ofstream file("election_config.txt");

    file << audit.electionType << "\n";
    file << audit.electionTitle << "\n";
    file << audit.electionPurpose << "\n";
}

void configureElection(AuditStack &audit)
{
    if (loadElectionConfig(audit))
    {
        cout << "Saved election configuration loaded.\n";
        addAuditEvent(audit, "Loaded election: " + audit.electionTitle);
        return;
    }

    int choice = 0;

    cout << "======= SET UP ELECTION =======\n";
    cout << "1. School election\n";
    cout << "2. College election\n";
    cout << "3. Product / feature poll\n";
    cout << "4. Company / employee poll\n";
    cout << "5. General-purpose election\n";
    cout << "Choose election type: ";

    while (!(cin >> choice) || choice < 1 || choice > 5)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter a number from 1 to 5: ";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1)
    {
        audit.electionType = "School";
    }
    else if (choice == 2)
    {
        audit.electionType = "College";
    }
    else if (choice == 3)
    {
        audit.electionType = "Product Poll";
    }
    else if (choice == 4)
    {
        audit.electionType = "Company";
    }
    else
    {
        audit.electionType = "General";
    }

    cout << "Enter election title: ";
    getline(cin, audit.electionTitle);

    cout << "Enter the exact purpose/question: ";
    getline(cin, audit.electionPurpose);

    if (audit.electionTitle.empty())
    {
        audit.electionTitle = audit.electionType + " Election";
    }

    if (audit.electionPurpose.empty())
    {
        audit.electionPurpose = "Choose one of the listed options.";
    }

    saveElectionConfig(audit);

    addAuditEvent(audit, "Configured election: " + audit.electionTitle);

    cout << "Election configuration saved.\n";
}

void showElectionInfo(AuditStack &audit)
{
    cout << "\nElection type: " << audit.electionType << "\n";
    cout << "Title: " << audit.electionTitle << "\n";
    cout << "Purpose: " << audit.electionPurpose << "\n";
}

void addAuditEvent(AuditStack &audit, string event)
{
    audit.events.push(event);
}

void showAuditEvents(AuditStack &audit)
{
    if (audit.events.empty())
    {
        cout << "No audit events in this session.\n";
        return;
    }

    stack<string> copy = audit.events;

    cout << "\n--- Audit Events (latest first) ---\n";

    while (!copy.empty())
    {
        cout << "- " << copy.top() << "\n";
        copy.pop();
    }
}
