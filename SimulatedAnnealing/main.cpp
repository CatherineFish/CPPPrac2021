#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>

#include "mainAlgorithm.h"
#include "solution.h"
#include "mutation.h"
#include "temperature.h"


std::vector<oneJob *> parser(std::string filename) {

    std::string line;
    std::vector<oneJob *> allJobs;
    size_t i;
    ifstream file (filename);
    while (std::getline(infile, line))
    {
        std::istringstream jobLine(line);
        int jobDur, dependence;
        jobLine >> jobDur;
        oneJob* newJob = new oneJob(jobDur, i);
        i++;
        while (jobLine >> dependence) {
            newJob->depNum.push_back(dependence);
        }
        allJobs.push_back(newJob);
    }
    return allJobs;
}

int compare (void * a, void * b)
{
    oneJob* arg1 = *(oneJob**)(a);
    oneJob* arg2 = *(oneJob**)(b);
    for (auto dep_1: arg1->depNum) {
        if (dep_1 == b->getNum()) {
            return -1;
        }
    }

    for (auto dep_2: arg2->depNum) {
        if (dep_2 == a->getNum()) {
            return 1;
        }
    }
    return 0;
}


solution * initSol (std::vector<oneJob*> allJobs, int procNum) {
    std::vector<std::vector<oneJob *>> initSol;
    std::vector<std::vector<oneJob *>> jobsGroups;
    for (size_t i = 0; i < allJobs.size(); i++) {
        bool isInGroup = false;
        for (size_t j = 0; j < jobsGroups.size(); j++) {
            for (size_t k = 0; k < jobsGroups[j].size(); j++) {
                for (auto groupDep: jobsGroups[j][k]) {
                    if (groupDep == allJobs[i]->getNum()) {
                        jobsGroups[j].push_back(allJobs[i]);
                        isInGroup = true;
                    }
                }
                if (!isInGroup) {
                    for (auto jobDep: allJobs[i]->depNum) {
                        if (jobDep == jobsGroups[j][k]->getNum){
                            jobsGroups[j].push_back(allJobs[i]);
                            isInGroup = true;
                        }
                    }
                }
            }
            
        }
        if (!isInGroup) {
            std::vector<oneJob *> newGroup;
            newGroup.push_back(allJobs[i]);
            jobsGroups.push_back(newGroup);
        }
    }
    for (size_t i = 0; i < jobsGroups.size(); i++) {
        std::qsort(
            jobsGroups[i].begin(), 
            jobsGroups[i].size(),
            sizeof(decltype(jobsGroups[i])::value_type),
            compare);
    }

    for(auto job: allJobs) {
        std::vector<oneJob*> curDep;
        for (auto k: job->depNum)
        {
            curDep.push_back(allJobs[k]);
        }
        job->initializeJob(curDep);
    }
    for (size_t j = 0; j < jobsGroups; j++) {
        double time = 0.0;
        for (size_t i = 0; i < jobsGroups[j].size(); i++)
        {
            jobsGroups[j][i]->setTStart(time);
            time += jobsGroups[j][i]->getDuration();
        }    
    }
    

    return new solution(procNum, initSol);
}

int main()
{
        
    oneJob *j_0, *j_1, *j_2, *j_3, *j_4;
    j_0 = new oneJob(100.0, 0, {});
    j_1 = new oneJob(50.0, 1, {0, 2});
    j_2 = new oneJob(12.0, 2, {0});
    j_3 = new oneJob(8888.0, 3, {4});
    j_4 = new oneJob(70000.0, 4, {});
    
    j_0->initializeJob({});
    j_1->initializeJob({j_0, j_2});
    j_2->initializeJob({j_0});
    j_3->initializeJob({j_4});
    j_4->initializeJob({});
    std::vector<std::vector<oneJob *>> initSol;
    std::vector<oneJob *> first;
    first.push_back(j_0);
    first.push_back(j_2);
    first.push_back(j_1);
    first.push_back(j_4);
    first.push_back(j_3);
    initSol.push_back(first);
    initSol.push_back(std::vector<oneJob *>());
    initSol.push_back(std::vector<oneJob *>());
    solution *sol;
    double time = 0.0;
    for (size_t i = 0; i < first.size(); i++)
    {
        first[i]->setTStart(time);
        time += first[i]->getDuration();
    }
    sol = new solution(3, initSol);
    mutation * mut;
    mut = new mutation();
    mainAlgorithm alg(sol, 1000, mut);
    sol->print();
    solution *res = alg.mainCycle();
    std::cout << "RESULT" << std::endl;
    res->print();
}