#include <iostream>
#include <fstream>
#include <sstream>
#include "candidate.h"

using namespace std;

void loadCandidates(CandidateManager &manager)
{
    manager.list.clear();

    ifstream file("candidates.txt");
    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        Candidate candidate;
        string id, votes;

        if (getline(ss, id, '|') &&
            getline(ss, candidate.name, '|') &&
            getline(ss, candidate.label, '|') &&
            getline(ss, votes, '|'))
        {
            try
            {
                candidate.id = stoi(id);
                candidate.votes = stoi(votes);
                manager.list.push_back(candidate);
            }
            catch (...)
            {
                // Ignore an incorrectly formatted record.
            }
        }
    }

    if (manager.list.empty())
    {
        Candidate first = {1, "Option_A", "Group_A", 0};
        Candidate second = {2, "Option_B", "Group_B", 0};
        Candidate third = {3, "Option_C", "Group_C", 0};

        manager.list.push_back(first);
        manager.list.push_back(second);
        manager.list.push_back(third);

        saveCandidates(manager);
    }
}

void saveCandidates(CandidateManager &manager)
{
    ofstream file("candidates.txt");

    for (int i = 0; i < (int)manager.list.size(); i++)
    {
        file << manager.list[i].id << "|"
             << manager.list[i].name << "|"
             << manager.list[i].label << "|"
             << manager.list[i].votes << "\n";
    }
}

void showCandidates(CandidateManager &manager)
{
    loadCandidates(manager);

    cout << "\n--- Candidates / Options ---\n";
    cout << "ID\tName\t\tGroup/Label\n";

    for (int i = 0; i < (int)manager.list.size(); i++)
    {
        cout << manager.list[i].id << "\t"
             << manager.list[i].name << "\t\t"
             << manager.list[i].label << "\n";
    }
}

int findCandidate(CandidateManager &manager, int id)
{
    for (int i = 0; i < (int)manager.list.size(); i++)
    {
        if (manager.list[i].id == id)
        {
            return i;
        }
    }

    return -1;
}

void addCandidate(CandidateManager &manager)
{
    loadCandidates(manager);

    Candidate candidate;

    cout << "Enter new option ID: ";

    if (!(cin >> candidate.id))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid ID.\n";
        return;
    }

    if (findCandidate(manager, candidate.id) != -1)
    {
        cout << "This ID already exists.\n";
        return;
    }

    cout << "Enter option name (use underscores for spaces): ";
    cin >> candidate.name;

    cout << "Enter group/label: ";
    cin >> candidate.label;

    candidate.votes = 0;
    manager.list.push_back(candidate);

    saveCandidates(manager);

    cout << "Option added successfully.\n";
}

bool countVote(CandidateManager &manager, int id)
{
    loadCandidates(manager);

    int index = findCandidate(manager, id);

    if (index == -1)
    {
        return false;
    }

    manager.list[index].votes++;

    saveCandidates(manager);

    return true;
}

void showResults(CandidateManager &manager)
{
    loadCandidates(manager);

    cout << "\n--- Election Results ---\n";

    for (int i = 0; i < (int)manager.list.size(); i++)
    {
        cout << manager.list[i].id << ". "
             << manager.list[i].name << " - "
             << manager.list[i].votes << " vote(s)\n";
    }
}
