#pragma once
#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include "random.h"

class oneJob
{
private:
	int duration, tStart = 0;
	size_t num;
public:
	std::vector<oneJob *> dependences = {};
	std::vector<size_t> depNum;
	int proc;
	
	oneJob(int dur, size_t num_): duration(dur), num(num_) 
	{}

	oneJob(int dur, size_t num_, std::vector<size_t> depNum_): duration(dur), num(num_) 
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

	int getDuration ()
	{
		return duration;
	}

	int getTStart ()
	{
		return tStart;
	}
	
	size_t getNum ()
	{
		return num;
	}
	
	int getLastTStart()
	{
		int time = 0;
		for (auto i: dependences)
		{
			time = std::max(time, i->getTStart() + i->getDuration());
		}
		return time;
	}

	void setTStart(int time)
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
	
	int getCriterion()
	{
		int Tmax = 0, Tmin = -1, Tcur, tLast = 0;
		for (size_t i = 0; i < procNum; i++)
		{
			if (sol[i].size()) {
				Tcur = sol[i][sol[i].size() - 1]->getTStart() + sol[i][sol[i].size() - 1]->getDuration();
				if (Tcur > Tmax)
				{
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

	size_t emptyTask(int procNum_, int tStart, oneJob *curJob)
	{
		
		int lastTime = 0;
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
		std::multimap<int, oneJob *> allJobs;
		
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
};
