#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "Election.h"
using namespace std;

void ElectionInfo::setup()
{
    ifstream input("election.txt");

    if (getline(input, type) && getline(input, title) && getline(input, purpose))
    {
        cout << "Existing election configuration loaded.\n";
        return;
    }

    int choice;

    cout << "\n--- Select Election Type ---\n";
    cout << "1. School election\n";
    cout << "2. College election\n";
    cout << "3. Company election\n";
    cout << "4. Product poll\n";
    cout << "5. Community election\n";
    cout << "6. General election / Other\n";
    cout << "Enter choice: ";

    while (!(cin >> choice) || choice < 1 || choice > 6)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Please enter 1 to 6: ";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1) type = "School";
    else if (choice == 2) type = "College";
    else if (choice == 3) type = "Company";
    else if (choice == 4) type = "Product Poll";
    else if (choice == 5) type = "Community";
    else type = "General / Other";

    cout << "Election title: ";
    getline(cin, title);

    cout << "Purpose / question: ";
    getline(cin, purpose);

    ofstream output("election.txt");
    output << type << "\n" << title << "\n" << purpose << "\n";

    cout << "Election setup saved.\n";
}

void ElectionInfo::show()
{
    cout << "\nType: " << type << "\n";
    cout << "Election: " << title << "\n";
    cout << "Purpose: " << purpose << "\n";
}

void Election::castVote(VoterManager &voters, CandidateManager &candidates,
                        VoteQueue &queue, AuditStack &audit)
{
    string id, pin;
    int optionID;

    cout << "Voter ID: ";
    cin >> id;
    cout << "PIN: ";
    cin >> pin;

    if (!voters.checkPIN(id, pin))
    {
        cout << "ID/PIN incorrect.\n";
        audit.push("Voter authentication failed: " + id);
        return;
    }

    if (voters.hasVoted(id))
    {
        cout << "This voter has already voted.\n";
        audit.push("Duplicate vote blocked: " + id);
        return;
    }

    // Demonstration OTP: displayed in console, not sent by SMS.
    srand((unsigned int)time(0));
    int otp = 1000 + rand() % 9000;
    cout << "Demo OTP: " << otp << "\n";

    int enteredOTP;
    cout << "Enter OTP: ";
    cin >> enteredOTP;

    if (enteredOTP != otp)
    {
        cout << "OTP verification failed.\n";
        audit.push("OTP verification failed: " + id);
        return;
    }

    candidates.showCandidates();
    cout << "Candidate/option ID: ";
    cin >> optionID;

    if (queue.isFull())
    {
        cout << "Queue is full.\n";
        return;
    }

    queue.enqueue(id, optionID);

    string queuedVoter;
    int queuedCandidate;
    queue.dequeue(queuedVoter, queuedCandidate);

    if (candidates.addVote(queuedCandidate))
    {
        voters.markVoted(queuedVoter);
        audit.push("Vote recorded for voter: " + queuedVoter);
        cout << "Vote recorded successfully.\n";
    }
    else
    {
        cout << "Invalid candidate/option.\n";
    }
}
