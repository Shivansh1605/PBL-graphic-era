#ifndef AUDIT_STACK_H
#define AUDIT_STACK_H

#include <stack>
#include <string>

class AuditStack {
private:
    std::stack<std::string> events;

public:
    void addEvent(const std::string& event);
    void showLatestEvents() const;
};

#endif
