#include "voter.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

VoterManager::VoterManager() {
    loadVoters();
}

void VoterManager::loadVoters() {
    voters.clear();
    ifstream file(fileName);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Voter voter;
        string voted;
        if (getline(ss, voter.voterID, '|') &&
            getline(ss, voter.name, '|') &&
            getline(ss, voter.pin, '|') &&
            getline(ss, voted, '|')) {
            voter.hasVoted = (voted == "1");
            voters.push_back(voter);
        }
    }
}

void VoterManager::saveVoters() const {
    ofstream file(fileName);
    for (const Voter& voter : voters) {
        file << voter.voterID << "|" << voter.name << "|"
             << voter.pin << "|" << (voter.hasVoted ? 1 : 0) << "\n";
    }
}

bool VoterManager::registerVoter() {
    string id, name, pin;

    cout << "\n--- Voter Registration ---\n";
    cout << "Enter voter ID (no spaces): ";
    cin >> id;

    if (findVoter(id) != nullptr) {
        cout << "This voter ID is already registered.\n";
        return false;
    }

    cout << "Enter full name (use underscores instead of spaces): ";
    cin >> name;

    cout << "Create a numeric PIN: ";
    cin >> pin;

    if (pin.empty()) {
        cout << "PIN cannot be empty.\n";
        return false;
    }
    for (char ch : pin) {
        if (ch < '0' || ch > '9') {
            cout << "PIN must contain digits only.\n";
            return false;
        }
    }

    voters.push_back({id, name, pin, false});
    saveVoters();
    cout << "Registration successful. Keep your voter ID and PIN private.\n";
    return true;
}

bool VoterManager::authenticate(const string& id, const string& pin) {
    Voter* voter = findVoter(id);
    return voter != nullptr && voter->pin == pin;
}

Voter* VoterManager::findVoter(const string& id) {
    for (Voter& voter : voters) {
        if (voter.voterID == id) return &voter;
    }
    return nullptr;
}

const vector<Voter>& VoterManager::getVoters() const {
    return voters;
}

void VoterManager::markVoted(const string& id) {
    Voter* voter = findVoter(id);
    if (voter != nullptr) {
        voter->hasVoted = true;
        saveVoters();
    }
}
