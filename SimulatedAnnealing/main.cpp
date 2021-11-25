#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <fstream>

#include "mainAlgorithm.h"
#include "solution.h"
#include "mutation.h"
#include "temperature.h"
#include "parallelAlg.h"

std::vector<std::vector<oneJob *>> jobsGroups;
    


std::vector<oneJob *> parser(std::string filename) {

    std::string line;
    std::vector<oneJob *> allJobs;
    size_t i = 0;
    int N;
    std::ifstream file(filename);
    while (std::getline(file, line)){
        if (line[0] == '#' || line[0] == ' ') continue;
        else {
            std::istringstream curLine(line);
            curLine >> N;    
            break;
        }
    }
    while (std::getline(file, line))
    {
        if (line[0] == '#' || line[0] == ' ') continue;
        //std::cout <<"WOW" << std::endl;
        std::istringstream jobLine(line);
        int jobDur, dependence;
        jobLine >> jobDur;

        //std::cout << "DUR: " << jobDur << std::endl;
        oneJob* newJob = new oneJob(jobDur, i);
        i++;
        //std::cout << "DEP: ";
        
        while (jobLine >> dependence) {
            if (dependence >= N) {
                std::cout << "Invalid input" << std::endl;
                exit(0);
            }
            newJob->depNum.push_back(dependence);
            //std::cout << dependence << " ";
        }
        //std::cout << std::endl;
        allJobs.push_back(newJob);
    }
    if (i < N) {
        std::cout << "Invalid input" << std::endl;
        exit(0);
    }
    //std::cout <<"YEA" << std::endl;
        
    return allJobs;
}

int compare (oneJob* arg1, oneJob * arg2)
{
    for (auto dep_1: arg1->depNum) {
        if (dep_1 == arg2->getNum()) {
            return 1;
        }
    }

    for (auto dep_2: arg2->depNum) {
        if (dep_2 == arg1->getNum()) {
            return 0;
        }
    }
    return 0;
}

void bubbleSort(int k)
{
    for (size_t i = 0; i < jobsGroups[k].size() - 1; i++)
    {
        for (size_t j = 0; j < jobsGroups[k].size() - 1 - i; j++)
        {
            if (compare(jobsGroups[k][j], jobsGroups[k][j + 1])){
                oneJob * job = jobsGroups[k][j + 1];
                jobsGroups[k].erase(jobsGroups[k].begin() + j + 1);
                jobsGroups[k].insert(jobsGroups[k].begin() + j, job);
            }
        }

    }    
     
    return;
}


solution * initSol (std::vector<oneJob*> allJobs, int procNum) {
    
    std::vector<std::vector<oneJob *>> initSol;
    for (size_t i = 0; i < allJobs.size(); i++) {
        //std::cout <<"JOB: " << allJobs[i]->getNum() << " GROUPS: " << jobsGroups.size() << std::endl;
        bool isInGroup = false;
        for (size_t j = 0; j < jobsGroups.size(); j++) {
            //std::cout << "       GROUP: " << j << std::endl;
            for (size_t k = 0; k < jobsGroups[j].size(); k++) {
                if (isInGroup) break;
                //std::cout << "SIZE: " << jobsGroups[j].size() <<std::endl;
                //std::cout << "          ELEM: "<< jobsGroups[j][k]->getNum() << std::endl; 
                //std::cout <<"            DEP: ";
                for (auto groupDep: jobsGroups[j][k]->depNum) {
                    //std::cout << groupDep << " ";
                    if (groupDep == allJobs[i]->getNum()) {
                        jobsGroups[j].push_back(allJobs[i]);
                        isInGroup = true;
                    }
                }
                //std::cout << std::endl;
                //std::cout << "BASE COND: " << isInGroup << std::endl;
                if (!isInGroup) {
                    for (auto jobDep: allJobs[i]->depNum) {
                        if (jobDep == jobsGroups[j][k]->getNum()){
                            jobsGroups[j].push_back(allJobs[i]);
                            isInGroup = true;
                        }
                    }
                }
                //std::cout <<"CHECK" << std::endl;
            }
            
        }
        //std::cout << " CONDITION: " << isInGroup << std::endl;
        if (!isInGroup) {
            std::vector<oneJob *> newGroup;
            newGroup.push_back(allJobs[i]);
            jobsGroups.push_back(newGroup);
            
        }
    }

    for (size_t i = 0; i < jobsGroups.size(); i++) {
        std::cout << "GROUP: " << i << std::endl;
        for (size_t j = 0; j < jobsGroups[i].size(); j++) {
            std::cout << jobsGroups[i][j]->getNum() << " ";
        }        
        std::cout << std::endl;
    }
    std::cout << "\n============================\n" << std::endl;
    
    for (size_t i = 0; i < jobsGroups.size(); i++) {
        bubbleSort(i);
    }


    for (size_t i = 0; i < jobsGroups.size(); i++) {
        std::cout << "GROUP: " << i << std::endl;
        for (size_t j = 0; j < jobsGroups[i].size(); j++) {
            std::cout << jobsGroups[i][j]->getNum() << " ";
        }        
        std::cout << std::endl;
    }
    std::cout << "\n============================\n" << std::endl;
    for(auto job: allJobs) {
        std::vector<oneJob*> curDep;
        for (auto k: job->depNum)
        {
            curDep.push_back(allJobs[k]);
        }
        job->initializeJob(curDep);
    }


    
    
    for (size_t i = 0; i < procNum; i++) {
        std::vector<oneJob * > newVec;
        initSol.push_back(newVec);
    }

    
    
    for (size_t j = 0; j < jobsGroups.size(); j++) {
        for (size_t i = 0; i < jobsGroups[j].size(); i++) {
            initSol[j % procNum].push_back(jobsGroups[j][i]);    
        }
            
    }

    for (size_t i = 0; i < initSol.size(); i++) {
        std::cout << "GROUP: " << i << std::endl;
        for (size_t j = 0; j < initSol[i].size(); j++) {
            std::cout << initSol[i][j]->getNum() << " : ";
            for (auto dep: initSol[i][j]->dependences) {
                std::cout << dep->getDuration() << " ";
            }
            std::cout << std::endl;
        }        
        std::cout << std::endl;
    }
    
    for (size_t j = 0; j < initSol.size(); j++) {
        double time = 0.0;
        for (size_t i = 0; i < initSol[j].size(); i++)
        {
            initSol[j][i]->setTStart(time);
            time += initSol[j][i]->getDuration();
        }    
    }    

    return new solution(procNum, initSol);
}

std::vector<std::vector<int>> parseShedule(std::string filename) {
    std::string line;
    std::vector<std::vector<int>> schedule;
    int N;
    size_t i = 0;
    std::ifstream file(filename);
    while (std::getline(file, line)){
        if (line[0] == '#' || line[0] == ' ') continue;
        else {
            std::istringstream curLine(line);
            curLine >> N;    
            break;
        }
    }
    while (std::getline(file, line))
    {
        if (line[0] == '#' || line[0] == ' ') continue;
        //std::cout <<"WOW" << std::endl;
        std::istringstream procLine(line);
        int procNum, newJob;
        char c;
        procLine >> procNum;
        procLine >> c;
        std::vector<int> newVect;
        
        i++;
        
        while (procLine >> newJob) {
            newVect.push_back(newJob);
        }
        //std::cout << std::endl;
        schedule.push_back(newVect);
    }
    if (i < N) {
        std::cout << "Invalid input" << std::endl;
        exit(0);
    }
    return schedule;
}

void scheduleCharact(std::string fileWithSchedule, std::string fileWithJobs) {
    std::vector<oneJob *> allJobs = parser(fileWithJobs);
    std::vector<std::vector<int>> schedule = parseShedule(fileWithSchedule);
    std::vector<std::vector<oneJob*>> jobSchedule;
    for (size_t i = 0; i < schedule.size(); i++) {
        std::vector<oneJob*> newElem;
        for (size_t j = 0; j < schedule[i].size(); j++) {
            newElem.push_back(allJobs[schedule[i][j]]);
            allJobs[schedule[i][j]]->proc = i;
        }
        jobSchedule.push_back(newElem);
    }

    for (size_t i = 0; i < allJobs.size() - 1; i++)
    {
        for (size_t j = 0; j < allJobs.size() - 1 - i; j++)
        {
            if (compare(allJobs[j], allJobs[j + 1])){
                oneJob * job = allJobs[j + 1];
                allJobs.erase(allJobs.begin() + j + 1);
                allJobs.insert(allJobs.begin() + j, job);
            }
        }
    }

    for (size_t i = 0; i < allJobs.size(); i++) {
        double mainStart = 0;
        for (size_t j = 0; j < schedule[allJobs[i]->proc].size(); j++) {
            jobSchedule[allJobs[i]->proc][j]->setTStart(std::max(mainStart, jobSchedule[allJobs[i]->proc][j]->getLastTStart()));
            mainStart = jobSchedule[allJobs[i]->proc][j]->getTStart() + jobSchedule[allJobs[i]->proc][j]->getDuration();
            if (jobSchedule[allJobs[i]->proc][j] == allJobs[i]) {
                break;
            }
        }
    }

    solution * sol = new solution(schedule.size(), jobSchedule);
    double disbalance = sol->getCriterion();
    double duration = sol->duration(); 
    double idling = sol->idle();
    std::cout << "Solution characteristics:" << std::endl;
    std::cout << "Duration : " << duration << std::endl;
    std::cout << "Disbalance : " << disbalance << std::endl;
    std::cout << "Idle time : " << idling << std::endl;
    return;

}

int main(int argc, char * argv[])
{
    std::srand(std::time(nullptr));
    
        
    /*oneJob *j_0, *j_1, *j_2, *j_3, *j_4;
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
    if (argc < 2){
        std::cout <<"Incorrect arguments" << std::endl;
        return 0;
    }
    int stepsForPrint = std::atoi(argv[1]);
    int procNum = 3;
    std::vector<oneJob *> allJobs = parser("strangeIn.txt");
    for (size_t i = 0; i < allJobs.size(); i++) {
        std::cout << allJobs[i]->getNum() << " Dur: " << allJobs[i]->getDuration() << " dependence: ";
        for (auto dep: allJobs[i]->depNum) {
            std::cout << dep << " ";
        }
        std::cout << std::endl;
    }    
    solution * sol = initSol(allJobs, procNum);
    std::cout << "\n============================\n" << std::endl;
    
    //sol->print();
    //sol = new solution(3, initSol);
    mutation * mut;
    mut = new mutation();
    mainAlgorithm alg(sol, 1000, mut, stepsForPrint);
    //sol->print();
    solution *res = alg.mainCycle();
    std::cout << "RESULT" << std::endl;
    res->print();
    res->printNum();
    if (argc < 2){
        std::cout <<"Incorrect arguments" << std::endl;
        return 0;
    }
    int nProc = std::atoi(argv[1]);
    */
    
    mutation * mut = new mutation();

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
    temperature *t = new temperature(1000);
    //sol->print();
    //parallelAlgorithm p(nProc);
    //solution* res = p.run(sol, t, mut);
    
    //mainAlgorithm alg(sol, t, mut);
    //solution *res = alg.mainCycle();
    //std::cout << "RESULT" << std::endl;
    //res->print();
    //res->printNum();

    
    int th_am;

    std::cout << "Please, print amoint of threads:" << std::endl;
    std::cin >> th_am;
    ParallelSimulating sim1(th_am, sol, t, mut);
    solution * res = sim1.ParralelSolution();
    res->print();
    
}