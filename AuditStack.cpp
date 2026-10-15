#include "AuditStack.h"
#include <iostream>

using namespace std;

void AuditStack::addEvent(const string& event) {
    events.push(event);
}

void AuditStack::showLatestEvents() const {
    if (events.empty()) {
        cout << "No audit events recorded during this session.\n";
        return;
    }

    stack<string> copy = events;
    cout << "\n--- Recent Audit Events (latest first) ---\n";
    while (!copy.empty()) {
        cout << "- " << copy.top() << "\n";
        copy.pop();
    }
}
