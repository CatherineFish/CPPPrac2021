#pragma once
#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include "random.h"
#include "job.h"

int compare (oneJob* arg1, oneJob * arg2) {
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


class solution
{

private:
    size_t procNum;
    std::vector<std::vector<oneJob *>> sol;

public:
    ~solution(){
        for (size_t i = 0; i < sol.size(); i++) {
            for (size_t j = 0; j < sol[i].size(); j++) {
                if (sol[i][j] != nullptr) 
                    delete sol[i][j];
            }        
        }
        std::vector<std::vector<oneJob *>>().swap(sol);
    } 
    solution(size_t procNum_, std::vector<std::vector<oneJob *>> sol_): procNum(procNum_){
        for (size_t i = 0; i < procNum; i++) {
            sol.emplace_back(std::vector<oneJob *>());
            for (size_t j = 0; j < sol_[i].size(); j++) {
                sol[i].emplace_back(sol_[i][j]);
            }
        }
        
    }
    
    int getCriterion()
    {
        int Tmax = 0, Tmin = -1, Tcur, tLast = 0;
        for (size_t i = 0; i < procNum; i++)
        {
            if (sol[i].size()) {
                Tcur = sol[i][sol[i].size() - 1]->getTStart() + sol[i][sol[i].size() - 1]->getDuration();
                if (Tcur > Tmax)
                {
                    if (Tmax && Tmin < 0) {
                        Tmin = Tmax;
                    }
                    Tmax = Tcur;
                }
                else if (Tmin < 0 || Tcur < Tmin)
                {
                    Tmin = Tcur;
                }
            } else {
                Tmin = 0;
            }
        }
        if (Tmin < 0)
        {
            Tmin = 0;
        }
        return Tmax - Tmin; 
    }

    int duration()
    {
        int Tmax = 0, Tcur, tLast = 0;
        for (size_t i = 0; i < procNum; i++)
        {
            if (sol[i].size()) {
                Tcur = sol[i][sol[i].size() - 1]->getTStart() + sol[i][sol[i].size() - 1]->getDuration();
                if (Tcur > Tmax)
                {
                    Tmax = Tcur;
                }
                
            } 
            
        }
        
        return Tmax; 
    }

    int idle()
    {
        int sum = 0.0, time = 0.0;
        int max = this->duration();
        for (size_t i = 0; i < procNum; i++)
        {
            time = 0.0;
            for (size_t j = 0; j < sol[i].size(); j++) {
                sum += sol[i][j]->getTStart() - time;
                time = sol[i][j]->getTStart() + sol[i][j]->getDuration();
            }
            sum += max - time;
        }
        return sum; 
    }   

    solution * copyOfObj()
    {
        std::vector<std::vector<oneJob *>> newSol;
        std::map<size_t, oneJob*> allJobs;

        for (size_t i = 0; i < procNum; i++) {
            for (size_t j = 0; j < sol[i].size(); j++) {
                allJobs.insert({sol[i][j]->getNum(), new oneJob(sol[i][j]->getDuration(), sol[i][j]->getNum(), sol[i][j]->depNum)});
                allJobs[sol[i][j]->getNum()]->setTStart(sol[i][j]->getTStart());
            }
        }

        for (std::map<size_t, oneJob *>::iterator it = allJobs.begin(); it != allJobs.end(); ++it) {
            std::vector<oneJob*> curDep;
            for (auto k: it->second->depNum)
            {
                curDep.push_back(allJobs[k]);
            }
            it->second->initializeJob(curDep);
        }

        for(size_t ii = 0; ii < sol.size(); ii++){
            newSol.push_back(std::vector<oneJob*>());
            for (size_t jj = 0; jj < sol[ii].size(); jj++){
                newSol[ii].push_back(allJobs[sol[ii][jj]->getNum()]);
            }
        }
        solution * res = new solution(procNum, newSol);
        /*for (size_t i = 0; i < newSol.size(); i++) {
            for (size_t j = 0; j < newSol[i].size(); j++) {
                if (newSol[i][j] != nullptr) {
                    delete newSol[i][j]; 
                }
            }
        }*/ 
        //std::vector<std::vector<oneJob *>>().swap(newSol);
        return res;
    }

    int getProcNum()
    {
        return procNum;
    }

    size_t jobsNumOnProc(int numOfProc)
    {
        return sol[numOfProc].size();
    }

    size_t emptyTask(int procNum_, int tStart, oneJob *curJob)
    {
        
        int lastTime = 0;
        int j = -1, k = sol[procNum_].size() - 1;
        if (k < 0) k = 0;
        if (!k) k = 1;
        for (size_t i = 0; i < sol[procNum_].size(); i++)
        {
            if (j == -1 && sol[procNum_][i]->getTStart() > tStart)
            {
                j = i;
            }
            if (j != -1) {
                for (auto job: sol[procNum_][i]->dependences)
                {
                    if (job == curJob) {
                        k = i;
                        break;
                    }
                }
                if (k != sol[procNum_].size() - 1)
                {
                    break;
                }
            }
        }
        if (j == -1) {
            return sol[procNum_].size();
        }
        return getRandom<int>(j, k);
    }

    oneJob* eraseJob(size_t numOfProc, int numOfjob)
    {
        oneJob * j = sol[numOfProc][numOfjob];
        sol[numOfProc].erase(sol[numOfProc].begin() + numOfjob);
        return j;
    }

    void check(){

        std::multimap<int, oneJob *> allJobs_map;
        
        for (size_t i = 0; i < sol.size(); i++)
        {
            for (size_t j = 0; j < sol[i].size(); j++)
            {
                allJobs_map.insert({sol[i][j]->getNum(), sol[i][j]});
            }
        }
        //std::cout << allJobs_map.size() << std::endl;
        std::vector<oneJob *> allJobs;

        for (std::multimap<int, oneJob *>::iterator it = allJobs_map.begin(); it != allJobs_map.end(); ++it) {
            allJobs.push_back(it->second);
        }
        

        for (size_t i = 0; i < sol.size(); i++) {
            for (size_t j = 0; j < sol[i].size(); j++) {
        //        std::cout << "i = " << i << " j = " << j << std::endl;
        //        std::cout << sol[i][j]->getNum() << std::endl;
                allJobs[sol[i][j]->getNum()]->proc = i;
            }
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
            int mainStart = 0;    
            for (size_t j = 0; j < sol[allJobs[i]->proc].size(); j++) {
                sol[allJobs[i]->proc][j]->setTStart(std::max(mainStart, sol[allJobs[i]->proc][j]->getLastTStart()));
                mainStart = sol[allJobs[i]->proc][j]->getTStart() + sol[allJobs[i]->proc][j]->getDuration();
                
            }
        }
    }

    void update(size_t numOfProc, std::multimap <int, oneJob *> allJobs)
    {
        
        for (std::multimap<int, oneJob *>::iterator it = allJobs.begin(); it != allJobs.end(); ++it) {
            it->second->setTStart(it->second->getLastTStart());
        }

        int lastTime = 0;
        for (size_t i = 0; i < sol.size(); i++)
        {
            lastTime = 0;
            for (size_t j = 0; j < sol[i].size(); j++)
            {
                sol[i][j]->setTStart(std::max(sol[i][j]->getTStart(), lastTime));
                lastTime = sol[i][j]->getTStart() + sol[i][j]->getDuration();
            }
        }
        this->check();
        return;
    }

    void insertJob(oneJob* j, size_t numOfProc, size_t numOfjob, bool flag)
    {
        if (!flag) {
            if (numOfjob == sol[numOfProc].size()) {
                sol[numOfProc].push_back(j);    
            } else {
                sol[numOfProc].insert(sol[numOfProc].begin() + numOfjob, j);    
            }
            return;
        }
        if (numOfjob == sol[numOfProc].size()) {
            sol[numOfProc].push_back(j);    
        } else {
            sol[numOfProc].insert(sol[numOfProc].begin() + numOfjob, j);    
        }
        /*std::multimap<int, oneJob *> allJobs;
        
        for (size_t i = 0; i < sol.size(); i++)
        {
            for (size_t j = 0; j < sol[i].size(); j++)
            {
                allJobs.insert({sol[i][j]->getTStart(), sol[i][j]});
            }
        }
        if (numOfjob == sol[numOfProc].size()) {
            sol[numOfProc].push_back(j);    
        } else {
            sol[numOfProc].insert(sol[numOfProc].begin() + numOfjob, j);    
        }
        int last = 0;
        for (size_t i = 0; i < sol[numOfProc].size(); i++)
        {
            sol[numOfProc][i]->setTStart(std::max(last, sol[numOfProc][i]->getLastTStart()));
            last = sol[numOfProc][i]->getTStart() + sol[numOfProc][i]->getDuration();   
        }
        this->update(numOfProc, allJobs);
        */
        this->check();
        return;
    }

    int getLastTStart(size_t numOfProc, size_t numOfjob)
    {
        return sol[numOfProc][numOfjob]->getLastTStart();
    }

    int getDuration(size_t numOfProc, size_t numOfjob)
    {
        return sol[numOfProc][numOfjob]->getDuration();
    }

    void print()
    {
        for (size_t i = 0; i < sol.size(); i++)
        {
            std::cout << "Processor #" << i << ": ";
            for (size_t j = 0; j < sol[i].size(); j++)
            {
                std::cout << "( t_start = " << sol[i][j]->getTStart() << " dur = " << sol[i][j]->getDuration() << " ), ";
            }
            std::cout << std::endl; 
        }
    }

    void printNum()
    {
        for (size_t i = 0; i < sol.size(); i++)
        {
            std::cout << "Processor #" << i << ": ";
            for (size_t j = 0; j < sol[i].size(); j++)
            {
                std::cout << sol[i][j]->getNum() << " ";
            }
            std::cout << std::endl; 
        }
    }

    void printNumFile(std::string filename)
    {
        std::ofstream file(filename, std::ios::app);
        
        file << sol.size() << std::endl;
        for (size_t i = 0; i < sol.size(); i++)
        {
            file << i << ": ";
            for (size_t j = 0; j < sol[i].size(); j++)
            {
                file << sol[i][j]->getNum() << " ";
            }
            file << std::endl; 
        }
    }

    void printRESULTNumFile(std::string filename)
    {
        std::ofstream file(filename);
        
        file << sol.size() << std::endl;
        for (size_t i = 0; i < sol.size(); i++)
        {
            file << i << ": ";
            for (size_t j = 0; j < sol[i].size(); j++)
            {
                file << sol[i][j]->getNum() << " ";
            }
            file << std::endl; 
        }
    }


};
