#include "candidate.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
CandidateManager::CandidateManager(){loadCandidates();}
void CandidateManager::loadCandidates(){
    candidates.clear();ifstream f(fileName);string line;
    while(getline(f,line)){stringstream ss(line);Candidate c;string id,votes;
        if(getline(ss,id,'|')&&getline(ss,c.name,'|')&&getline(ss,c.party,'|')&&getline(ss,votes,'|')){
            try{c.candidateID=stoi(id);c.votes=stoi(votes);candidates.push_back(c);}catch(...){}
        }
    }
    if(candidates.empty()){candidates={{1,"Option_A","Group_A",0},{2,"Option_B","Group_B",0},{3,"Option_C","Group_C",0}};saveCandidates();}
}
void CandidateManager::saveCandidates()const{ofstream f(fileName);for(const auto& c:candidates)f<<c.candidateID<<"|"<<c.name<<"|"<<c.party<<"|"<<c.votes<<"\n";}
void CandidateManager::showCandidates()const{cout<<"\n--- Options for "<< "this election" <<" ---\nID\tName\t\tGroup/Label\n";for(const auto& c:candidates)cout<<c.candidateID<<"\t"<<c.name<<"\t\t"<<c.party<<"\n";}
bool CandidateManager::addCandidate(){int id;string name,party;cout<<"New option ID: ";if(!(cin>>id)){cin.clear();cin.ignore(10000,'\n');return false;}if(findCandidate(id)){cout<<"ID already exists.\n";return false;}cout<<"Option name (underscores for spaces): ";cin>>name;cout<<"Group/label: ";cin>>party;candidates.push_back({id,name,party,0});saveCandidates();cout<<"Added.\n";return true;}
Candidate* CandidateManager::findCandidate(int id){for(auto& c:candidates)if(c.candidateID==id)return &c;return nullptr;}
bool CandidateManager::addVote(int id){Candidate* c=findCandidate(id);if(!c)return false;++c->votes;saveCandidates();return true;}
void CandidateManager::showResults()const{cout<<"\n--- Election Results ---\n";for(const auto& c:candidates)cout<<c.candidateID<<". "<<c.name<<" ("<<c.party<<") - "<<c.votes<<" vote(s)\n";}
