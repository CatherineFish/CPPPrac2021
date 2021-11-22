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
	
public:
	std::vector<oneJob *> dependences = {};
	oneJob(double dur) 
	{
		duration = dur;
		
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

	
	double getLastTStart()
	{
		double time = 0.0;
		for (auto i: dependences)
		{
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
		return new solution(*this);
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
		size_t j = 0, k = std::min(size_t(0), sol[procNum_].size() - 1);
		for (size_t i = 0; i < sol[procNum_].size(); i++)
		{
			if (!j && sol[procNum_][i]->getTStart() > tStart)
			{
				j = i;
			}
			if (j) {
				std::cout << "CHECK " << sol[procNum_][i]->getDuration() << "dep = " << sol[procNum_][i]->dependences.size() << std::endl;
				for (auto job: sol[procNum_][i]->dependences)
				{
					std::cout << "dur = " << job->getDuration() << std::endl;
					if (job == curJob) {
						std::cout << "YES" << std::endl;
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
		std::cout << "j = " << j << " k = " << k << std::endl;
		return getRandom<int>(j, k);
	}

	oneJob* eraseJob(size_t numOfProc, int numOfjob)
	{
		oneJob * j = sol[numOfProc][numOfjob];
		sol[numOfProc].erase(sol[numOfProc].begin() + numOfjob);
		std::cout << "AFTER ERASE: " << std::endl;
		this->print();
		return j;
	}

	void update(size_t numOfProc)
	{
		std::map<oneJob *, double> allJobs;
		for (size_t i = 0; i < sol.size(); i++)
		{
			for (size_t j = 0; j < sol[i].size(); j++)
			{
				allJobs.insert({sol[i][j], sol[i][j]->getTStart()});
			}
		}
		for(std::map<oneJob *, double>::iterator it = allJobs.begin(); it != allJobs.end(); ++it) {
			 it->first->setTStart(std::max(it->first->getLastTStart(), it->first->getTStart()));
		}

		double lastTime = 0;
		for (size_t i = 0; i < sol.size(); i++)
		{
			if (i == numOfProc) continue;
			for (size_t j = 0; j < sol[i].size(); j++)
			{
				sol[i][j]->setTStart(std::max(sol[i][j]->getLastTStart(), std::max(sol[i][j]->getTStart(), lastTime)));
				lastTime = sol[i][j]->getTStart() + sol[i][j]->getDuration();
			}
		}
		return;
	}

	void insertJob(oneJob* j, size_t numOfProc, size_t numOfjob)
	{
		sol[numOfProc].insert(sol[numOfProc].begin() + numOfjob, j);
		std::cout << "AFTER INSERT" << std::endl;
		//j->setTStart(std::max(sol[numOfProc][numOfjob - 1]->getTStart() + sol[numOfProc][numOfjob - 1]->getDuration(), lastTStart));
		std::cout << "PROC NUM: " << numOfProc << std::endl;
		double last = 0;
		for (size_t i = 0; i < sol[numOfProc].size(); i++)
		{
			std::cout << i << " last = " << last << " lastTstart = " << sol[numOfProc][i]->getLastTStart() << std::endl;
			sol[numOfProc][i]->setTStart(std::max(last, sol[numOfProc][i]->getLastTStart()));
			last = sol[numOfProc][i]->getTStart() + sol[numOfProc][i]->getDuration();	
		}
		this->print();
		this->update(numOfProc);
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
