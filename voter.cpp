#include <iostream>
#include <fstream>
#include <sstream>
#include "voter.h"

using namespace std;

void loadVoters(VoterManager &manager)
{
    manager.list.clear();

    ifstream file("voters.txt");
    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        Voter voter;
        string voted;

        if (getline(ss, voter.id, '|') &&
            getline(ss, voter.name, '|') &&
            getline(ss, voter.pin, '|') &&
            getline(ss, voted, '|'))
        {
            voter.hasVoted = (voted == "1");
            manager.list.push_back(voter);
        }
    }
}

void saveVoters(VoterManager &manager)
{
    ofstream file("voters.txt");

    for (int i = 0; i < (int)manager.list.size(); i++)
    {
        file << manager.list[i].id << "|"
             << manager.list[i].name << "|"
             << manager.list[i].pin << "|"
             << (manager.list[i].hasVoted ? 1 : 0) << "\n";
    }
}

int findVoter(VoterManager &manager, string id)
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

void registerVoter(VoterManager &manager)
{
    loadVoters(manager);

    Voter voter;

    cout << "\n--- Voter Registration ---\n";
    cout << "Enter voter ID (no spaces): ";
    cin >> voter.id;

    if (findVoter(manager, voter.id) != -1)
    {
        cout << "This voter ID is already registered.\n";
        return;
    }

    cout << "Enter name (use underscores for spaces): ";
    cin >> voter.name;

    cout << "Create a numeric PIN: ";
    cin >> voter.pin;

    if (voter.pin.empty())
    {
        cout << "PIN cannot be empty.\n";
        return;
    }

    for (int i = 0; i < (int)voter.pin.length(); i++)
    {
        if (voter.pin[i] < '0' || voter.pin[i] > '9')
        {
            cout << "PIN must contain digits only.\n";
            return;
        }
    }

    voter.hasVoted = false;
    manager.list.push_back(voter);

    saveVoters(manager);

    cout << "Registration successful.\n";
}

bool authenticateVoter(VoterManager &manager, string id, string pin)
{
    loadVoters(manager);

    int index = findVoter(manager, id);

    if (index == -1)
    {
        return false;
    }

    if (manager.list[index].pin == pin)
    {
        return true;
    }

    return false;
}

void markVoted(VoterManager &manager, int index)
{
    if (index >= 0 && index < (int)manager.list.size())
    {
        manager.list[index].hasVoted = true;
        saveVoters(manager);
    }
}
