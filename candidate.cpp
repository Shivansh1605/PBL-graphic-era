#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "Candidate.h"
using namespace std;

CandidateManager::CandidateManager()
{
    count = 0;
    load();
}

void CandidateManager::load()
{
    count = 0;
    ifstream file("candidates.txt");
    string line;

    while (getline(file, line) && count < 100)
    {
        stringstream ss(line);
        string id, vote;

        getline(ss, id, '|');
        getline(ss, names[count], '|');
        getline(ss, vote, '|');

        ids[count] = atoi(id.c_str());
        votes[count] = atoi(vote.c_str());
        count++;
    }

    if (count == 0)
    {
        ids[0] = 1; names[0] = "Option_A"; votes[0] = 0;
        ids[1] = 2; names[1] = "Option_B"; votes[1] = 0;
        count = 2;
        save();
    }
}

void CandidateManager::save()
{
    ofstream file("candidates.txt");

    for (int i = 0; i < count; i++)
        file << ids[i] << "|" << names[i] << "|" << votes[i] << "\n";
}

int CandidateManager::findCandidate(int id)
{
    for (int i = 0; i < count; i++)
        if (ids[i] == id)
            return i;

    return -1;
}

void CandidateManager::showCandidates()
{
    load();
    cout << "\nID\tCandidate / Option\n";

    for (int i = 0; i < count; i++)
        cout << ids[i] << "\t" << names[i] << "\n";
}

void CandidateManager::addCandidate()
{
    load();

    if (count >= 100)
    {
        cout << "Limit reached.\n";
        return;
    }

    int id;
    string name;

    cout << "New ID: ";
    cin >> id;

    if (findCandidate(id) != -1)
    {
        cout << "ID already exists.\n";
        return;
    }

    cout << "Name (no spaces): ";
    cin >> name;

    ids[count] = id;
    names[count] = name;
    votes[count] = 0;
    count++;

    save();
    cout << "Added successfully.\n";
}

bool CandidateManager::addVote(int id)
{
    load();
    int index = findCandidate(id);

    if (index == -1)
        return false;

    votes[index]++;
    save();
    return true;
}

void CandidateManager::results()
{
    load();
    cout << "\n--- Election Results ---\n";

    for (int i = 0; i < count; i++)
        cout << names[i] << " : " << votes[i] << " votes\n";
}
