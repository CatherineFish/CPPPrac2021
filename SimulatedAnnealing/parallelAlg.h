#pragma once
#include <time.h>
#include <mutex>
#include <thread>
#include <variant>
#include "mainAlgorithm.h"
#include <algorithm>


class ParallelSimulating{
private:
    int num_procs;
    solution* curSol;
    temperature* temp;
    mutation* curMutation;
    
    std::vector <int> inform;
    std::vector <solution *> worktask;
    std::mutex writelock;

public:
    ParallelSimulating(int procs, solution* initSol, temperature * initTemp, mutation* initMutation): num_procs(procs), curMutation(initMutation) {
        curSol = initSol->copyOfObj();
        temp = initTemp->copyOfObj();

    }

    void InitWorkTask( solution* best){
        mainAlgorithm sim(curSol, temp, curMutation, best);
        solution* sol = sim.mainCycle()->copyOfObj();
        writelock.lock();
        worktask.emplace_back(sol);
        writelock.unlock();
    };

    solution* ParralelSolution() {
        std::vector<std::thread> thread_vec(num_procs);
        solution* best = nullptr;
        int it = 0;

        while (it < 10) {

            for (size_t i = 0; i < num_procs; i++)
                thread_vec[i] = std::thread(&ParallelSimulating::InitWorkTask, this, best);

            for (auto &th: thread_vec)
                if (th.joinable())
                    th.join();
    
            if (best) {
                solution* new_solution = this->GetBestSolution();
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
                best = this->GetBestSolution()->copyOfObj();
                it=1;
            }
            thread_vec.clear();
            thread_vec.resize(num_procs);
            worktask.clear();
        }
        return best;

    }



    solution* GetBestSolution() {
        std::vector <int> allcrit;
        solution* tst = nullptr;
        for (auto &it: worktask)
            allcrit.emplace_back(it->getCriterion());

        int max = *std::max_element(allcrit.begin(), allcrit.end());
        for (auto it: worktask) {
            if (it->getCriterion() == max)
                tst = it->copyOfObj();
            delete(it);
        }
        return tst;
    }

};