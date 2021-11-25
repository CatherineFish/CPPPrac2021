#pragma once
#include <time.h>
#include <mutex>
#include <thread>
#include <variant>
#include "mainAlgorithm.h"
#include <algorithm>


class parallelAlgorithm{
private:
    int procNum;
    solution* curSol;
    temperature* temp;
    mutation* curMutation;
    
    std::vector <solution*> workTask;
    std::mutex writeLock;
    

public:
    parallelAlgorithm(int procs, solution* initSol, temperature * initTemp, mutation* initMutation): procNum(procs) {
        curSol = initSol->copyOfObj();
        temp = initTemp->copyOfObj();
        curMutation = initMutation->copyOfObj();
    }

    void initialization(solution* best){
        mainAlgorithm mainSol(curSol, temp, curMutation, best);
        solution* sol = mainSol.mainCycle()->copyOfObj();
        writeLock.lock();
        workTask.emplace_back(sol);
        writeLock.unlock();
    };

    solution* parralelSolution() {
        std::vector<std::thread> threadVec(procNum);
        solution* best = nullptr;
        int it = 0;

        while (it < 10) {
            for (size_t i = 0; i < procNum; i++)
                threadVec[i] = std::thread(&parallelAlgorithm::initialization, this, best);

            for (auto &th: threadVec)
                if (th.joinable())
                    th.join();
    
            if (best) {
                solution* new_solution = this->getBestSolution();
                if (new_solution->getCriterion() < best->getCriterion()) {
                    delete best;
                    best = new_solution->copyOfObj();
                    delete new_solution;
                    it = 0;
                } else {
                    delete new_solution;
                    it++;
                }
            } else {
                best = this->getBestSolution()->copyOfObj();
                it=1;
            }
            threadVec.clear();
            threadVec.resize(procNum);
            workTask.clear();
        }
        return best;
    }

    solution* getBestSolution() {
        std::vector <int> allCrit;
        solution* tst = nullptr;
        for (auto &it: workTask)
            allCrit.emplace_back(it->getCriterion());

        int max = *std::max_element(allCrit.begin(), allCrit.end());
        for (auto it: workTask) {
            if (it->getCriterion() == max)
                tst = it->copyOfObj();
            delete(it);
        }
        return tst;
    }
};