#include <iostream>
#include <fstream>
#include <sstream>
#include "Voter.h"
using namespace std;

VoterManager::VoterManager()
{
    count = 0;
    load();
}

void VoterManager::load()
{
    count = 0;
    ifstream file("voters.txt");
    string line;

    while (getline(file, line) && count < 100)
    {
        stringstream ss(line);
        string status;

        getline(ss, ids[count], '|');
        getline(ss, names[count], '|');
        getline(ss, pins[count], '|');
        getline(ss, status, '|');

        voted[count] = (status == "1");
        count++;
    }
}

void VoterManager::save()
{
    ofstream file("voters.txt");

    for (int i = 0; i < count; i++)
        file << ids[i] << "|" << names[i] << "|" << pins[i]
             << "|" << (voted[i] ? 1 : 0) << "\n";
}

int VoterManager::findVoter(string id)
{
    for (int i = 0; i < count; i++)
        if (ids[i] == id)
            return i;

    return -1;
}

void VoterManager::registerVoter()
{
    load();

    if (count >= 100)
    {
        cout << "Voter capacity reached.\n";
        return;
    }

    string id, name, pin;

    cout << "Voter ID: ";
    cin >> id;

    if (findVoter(id) != -1)
    {
        cout << "Voter already exists.\n";
        return;
    }

    cout << "Name (use underscore instead of spaces): ";
    cin >> name;

    cout << "Create PIN: ";
    cin >> pin;

    if (pin.length() < 4)
    {
        cout << "PIN must contain at least 4 digits.\n";
        return;
    }

    ids[count] = id;
    names[count] = name;
    pins[count] = pin;
    voted[count] = false;
    count++;

    save();
    cout << "Registration successful.\n";
}

bool VoterManager::checkPIN(string id, string pin)
{
    load();
    int index = findVoter(id);

    if (index == -1)
        return false;

    return pins[index] == pin;
}

bool VoterManager::hasVoted(string id)
{
    load();
    int index = findVoter(id);

    if (index == -1)
        return false;

    return voted[index];
}

void VoterManager::markVoted(string id)
{
    load();
    int index = findVoter(id);

    if (index != -1)
    {
        voted[index] = true;
        save();
    }
}
