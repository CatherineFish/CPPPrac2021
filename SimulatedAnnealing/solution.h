#pragma once
#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include "random.h"

class oneJob
{
private:
	double duration, tStart = 0.0;
	size_t num;
public:
	std::vector<oneJob *> dependences = {};
	std::vector<size_t> depNum;

	oneJob(double dur, size_t num_): duration(dur), num(num_) 
	{}

	oneJob(double dur, size_t num_, std::vector<size_t> depNum_): duration(dur), num(num_) 
	{
		for (auto i: depNum_){
			depNum.push_back(i);
		}
	}

	void initializeJob(std::vector<oneJob *> dep)
	{
		for (size_t i = 0; i < dep.size(); i++)
		{
			dependences.push_back(dep[i]);
		}
		return;
	}

	double getDuration ()
	{
		return duration;
	}

	double getTStart ()
	{
		return tStart;
	}
	size_t getNum ()
	{
		return num;
	}
	
	double getLastTStart()
	{
		//std::cout << "DUR: " << this->getDuration() << "DEP SIZE: " << dependences.size() << std::endl;
		double time = 0.0;
		//std::cout << "DEP" << std::endl;
		for (auto i: dependences)
		{
			//std::cout << "DURATION: " << i->getDuration() << " START" << i->getTStart() << std::endl;
			time = std::max(time, i->getTStart() + i->getDuration());
		}
		return time;
	}

	void setTStart(double time)
	{
		tStart = time;
		return;
	}

};



class solution
{

private:
	size_t procNum;
	std::vector<std::vector<oneJob *>> sol;
public:
	solution(size_t procNum_, std::vector<std::vector<oneJob *>> sol_): procNum(procNum_){
		for (size_t i = 0; i < procNum; i++) {
            sol.emplace_back(std::vector<oneJob *>());
        	for (size_t j = 0; j < sol_[i].size(); j++) {
            	sol[i].emplace_back(sol_[i][j]);
        	}
        }
        
	}
	
	~solution() = default;

	double getCriterion()
	{
		double Tmax = 0.0, Tmin = -1.0, Tcur, tLast = 0.0;
		for (size_t i = 0; i < procNum; i++)
		{
			if (sol[i].size()) {
				Tcur = sol[i][sol[i].size() - 1]->getTStart() + sol[i][sol[i].size() - 1]->getDuration();
				if (Tcur > Tmax)
				{
					Tmax = Tcur;
				}
				else if (Tmin < 0.0 || Tcur < Tmin)
				{
					Tmin = Tcur;
				}
			} else {
				Tmin = 0.0;
			}
			
		}
		if (Tmin < 0.0)
		{
			Tmin = 0.0;
		}
		return Tmax - Tmin; 
	}

	solution * copyOfObj()
	{
		std::vector<std::vector<oneJob *>> newSol;
		std::map<size_t, oneJob*> allJobs;

		for (size_t i = 0; i < procNum; i++) {
        	for (size_t j = 0; j < sol[i].size(); j++) {
        		allJobs.insert({sol[i][j]->getNum(), new oneJob(sol[i][j]->getDuration(), sol[i][j]->getNum(), sol[i][j]->depNum)});
        		allJobs[sol[i][j]->getNum()]->setTStart(sol[i][j]->getTStart());
        		//std::cout << "MAP " << allJobs[sol[i][j]->getNum()]->depNum.size() << std::endl;
        	}
        }

        for(std::map<size_t, oneJob *>::iterator it = allJobs.begin(); it != allJobs.end(); ++it) {
			//std::cout<<"======== NUM OF JOB = " << it->first <<" =========" << std::endl;
			//std::cout << "DEP SIZE = " << it->second->depNum.size() << std::endl;
			std::vector<oneJob*> curDep;
			for (auto k: it->second->depNum)
			{
				curDep.push_back(allJobs[k]);
			}
			//std::cout << "RESULT SIZE: " << curDep.size() << std::endl;
			it->second->initializeJob(curDep);
		}
		for(size_t ii = 0; ii < sol.size(); ii++){
			newSol.push_back(std::vector<oneJob*>());
			for (size_t jj = 0; jj < sol[ii].size(); jj++){
				newSol[ii].push_back(allJobs[sol[ii][jj]->getNum()]);
			}
		}

		return new solution(procNum, newSol);
	}

	int getProcNum()
	{
		return procNum;
	}

	size_t jobsNumOnProc(int numOfProc)
	{
		return sol[numOfProc].size();
	}

	size_t emptyTask(int procNum_, double tStart, oneJob *curJob)
	{
		
		double lastTime = 0.0;
		size_t j = -1, k = sol[procNum_].size() - 1;
		if (k < 0) k = 0;
		if (!k) k = 1;
		for (size_t i = 0; i < sol[procNum_].size(); i++)
		{
			if (j == -1 && sol[procNum_][i]->getTStart() > tStart)
			{
				j = i;
			}
			if (j != -1) {
				//std::cout << "CHECK " << sol[procNum_][i]->getDuration() << "dep = " << sol[procNum_][i]->dependences.size() << std::endl;
				for (auto job: sol[procNum_][i]->dependences)
				{
					//std::cout << "dur = " << job->getDuration() << std::endl;
					if (job == curJob) {
						//std::cout << "YES" << std::endl;
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
		std::cout << "j = " << j << " k = " << k << std::endl;
		return getRandom<int>(j, k);
	}

	oneJob* eraseJob(size_t numOfProc, int numOfjob)
	{
		oneJob * j = sol[numOfProc][numOfjob];
		sol[numOfProc].erase(sol[numOfProc].begin() + numOfjob);
		return j;
	}

	void update(size_t numOfProc, std::multimap <double, oneJob *> allJobs)
	{
		
		for(std::multimap<double, oneJob *>::iterator it = allJobs.begin(); it != allJobs.end(); ++it) {
			std::cout << "DUR = " << it->second->getDuration() << " StartTime = " << it->second->getTStart() << std::endl;
			it->second->setTStart(it->second->getLastTStart());
		}

		double lastTime = 0;
		for (size_t i = 0; i < sol.size(); i++)
		{
			//if (i == numOfProc) continue;
			lastTime = 0;
			for (size_t j = 0; j < sol[i].size(); j++)
			{
				//std::cout << "DUR: " << sol[i][j]->getDuration()
				sol[i][j]->setTStart(std::max(sol[i][j]->getTStart(), lastTime));
				lastTime = sol[i][j]->getTStart() + sol[i][j]->getDuration();
			}
		}
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
		std::multimap<double, oneJob *> allJobs;
		std::cout << "ALL JOBS" << std::endl;
		
		for (size_t i = 0; i < sol.size(); i++)
		{
			//std::cout << "PROC: " << i << std::endl;
			for (size_t j = 0; j < sol[i].size(); j++)
			{
				//std::cout << "JOB: " << sol[i][j]->getDuration() << std::endl; 
				allJobs.insert({sol[i][j]->getTStart(), sol[i][j]});
			}
		}
		if (numOfjob == sol[numOfProc].size()) {
			sol[numOfProc].push_back(j);	
		} else {
			sol[numOfProc].insert(sol[numOfProc].begin() + numOfjob, j);	
		}
		double last = 0;
		for (size_t i = 0; i < sol[numOfProc].size(); i++)
		{
			sol[numOfProc][i]->setTStart(std::max(last, sol[numOfProc][i]->getLastTStart()));
			last = sol[numOfProc][i]->getTStart() + sol[numOfProc][i]->getDuration();	
		}
		this->update(numOfProc, allJobs);
		return;
	}

	double getLastTStart(size_t numOfProc, size_t numOfjob)
	{
		return sol[numOfProc][numOfjob]->getLastTStart();
	}

	double getDuration(size_t numOfProc, size_t numOfjob)
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

};
