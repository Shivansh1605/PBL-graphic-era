#include "admin.h"
#include <iostream>
using namespace std;
Admin::Admin(VoterManager& vm,CandidateManager& cm,Election& e,AuditStack& log):voterManager(vm),candidateManager(cm),election(e),audit(log){}
void Admin::login(){
    string password;cout<<"Admin password: ";cin>>password;
    if(password!="admin123"){cout<<"Incorrect password.\n";audit.addEvent("Failed admin login");return;}
    audit.addEvent("Admin login successful");int choice=0;
    do{
        cout<<"\n--- Admin Panel ---\n1. View results\n2. Add candidate/option\n3. View audit events\n4. View election purpose\n5. Return\nChoice: ";
        if(!(cin>>choice)){cin.clear();cin.ignore(10000,'\n');cout<<"Enter a number.\n";continue;}
        switch(choice){case 1:election.showResults();break;case 2:candidateManager.addCandidate();break;case 3:election.showAudit();break;case 4:audit.showElectionInfo();break;case 5:break;default:cout<<"Invalid choice.\n";}
    }while(choice!=5);
}
