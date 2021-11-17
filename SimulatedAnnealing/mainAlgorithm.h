#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include <cstdlib>

template <typename T>
T getRandom(T min, T max) {
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<T>(rand() * fraction * (max - min) + min);
}


class oneJob
{
private:
	double duration, tStart = 0.0;
	std::vector<oneJob *> dependences ={};
	int num;
	
public:
	oneJob (double dur_, std::vector<oneJob*> dep_, int num_): duration(dur_), dependences(dep_), num(num_){}

	double getDuration ()
	{
		return duration;
	}

	double getTStart ()
	{
		return tStart;
	}

	int getNum ()
	{
		return num;
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
	int procNum;
	std::vector<std::vector<oneJob *>> sol;
public:
	solution(int procNum_, std::vector<std::vector<oneJob>> sol_): procNum(procNum_){
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
			Tcur = ((sol[i])[sol[i].size() - 1])->getTStart() + ((sol[i])[sol[i].size() - 1])->getDuration();
			if (Tcur > Tmax)
			{
				Tmax = Tcur;
			}
			if (Tmin < 0.0 || Tcur < Tmin)
			{
				Tmin = Tcur;
			}
			
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

	size_t emptyTask(int procNum_, double duration, double tStart)
	{
		double lastTime = 0.0;
		size_t j = 0;
		for (size_t i = 0; i < sol[procNum_].size(); i++)
		{
			if (sol[procNum_][i]->getTStart() > tStart)
			{
				j = i;
				break;
			}
		}
		return getRandom<int>(j, sol[procNum_].size());
	}

	oneJob* eraseJob(size_t numOfProc, int numOfjob)
	{
		oneJob * j = sol[numOfProc][numOfjob];
		sol[numOfProc].erase(sol[numOfProc].begin() + numOfjob);
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

	void insertJob(oneJob* j, size_t numOfProc, size_t numOfjob, double lastTStart)
	{
		sol[numOfProc].insert(sol[numOfProc].begin() + numOfjob, j);
		j->setTStart(std::max(sol[numOfProc][numOfjob - 1]->getTStart() + sol[numOfProc][numOfjob - 1]->getDuration(), lastTStart));
		double last = j->getTStart() + j->getDuration();
		for (size_t i = numOfjob + 1; i < sol[numOfProc].size(); i++)
		{
			if (last < sol[numOfProc][i]->getTStart())
			{
				break;
			} else {
				sol[numOfProc][i]->setTStart(last);
				last = last + sol[numOfProc][i]->getDuration();	
			}
		}
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

class mutation
{
public:
	~mutation() = default;

	solution* mutate(solution* sol)
	{
		size_t numOfProc = getRandom<int>(0, sol->getProcNum());
		while (sol->jobsNumOnProc(numOfProc) == 0)
		{
			numOfProc = getRandom<int>(0, sol->getProcNum());
		}
		size_t numOfjob = getRandom<int>(0, sol->jobsNumOnProc(numOfProc));
		double lastTStart = sol->getLastTStart(numOfProc, numOfjob); 
		size_t newNumOfProc = getRandom<int>(0, sol->getProcNum());
		while (sol->jobsNumOnProc(newNumOfProc) == 0)
		{
			newNumOfProc = getRandom<int>(0, sol->getProcNum());
		}

		size_t newNumOfjob = sol->emptyTask(newNumOfProc, sol->getDuration(numOfProc, numOfjob), lastTStart);		
		oneJob* j = sol->eraseJob(numOfProc, numOfjob);
		sol->insertJob(std::move(j), newNumOfProc, newNumOfjob, lastTStart);
		return sol;
	}
};


class temperature
{
private:
	double initTemp, curTemp;
	int iter;
	
public:
	temperature(double temp_ = 0) : initTemp(temp_), curTemp(temp_), iter(0) {}
	
	~temperature() = default;

	double getTemp ()
	{
		return curTemp;
	}


	void decreaseTemp()
	{
		iter++;
		curTemp = initTemp / log (1 + iter);
		return; 
	}
};



class mainAlgoritnhm
{

private:
	solution* curSol, *bestSol;
	temperature* temp;
	mutation* curMutation;
	int globOutMaxIter = 100, globInMaxIter = 20;
	double bestCriterion, curCriterion;
 

public:
	mainAlgoritnhm(solution* initSol, double initTemp, mutation* initMutation): curMutation(initMutation){
		curSol = initSol->copyOfObj();
		bestSol = initSol->copyOfObj();
		temp = new temperature(initTemp);
	} 
	
	~mainAlgoritnhm() {
		delete(curSol);
		delete(bestSol);
		delete(temp);
		delete(curMutation);
	}

	solution* mainCycle ()
	{
		int inMaxIter = globInMaxIter, outMaxIter = globOutMaxIter;
		curCriterion = curSol->getCriterion();
		bestCriterion = bestSol->getCriterion();
		while (outMaxIter--)
		{
			while (inMaxIter--)
			{
				solution * newSol = curMutation->mutate(curSol->copyOfObj());
				double newCriterion = newSol->getCriterion();
				// сначала проверить, лучше ли лучшего
				// потом сравниваем с текущем
				if (newCriterion < bestCriterion)
				{
					delete bestSol;
					bestSol = newSol->copyOfObj();
					bestCriterion = newCriterion;
					outMaxIter = globOutMaxIter;
				} 
				int diff = newCriterion - curCriterion;
				if (diff <= 0)
				{
					delete curSol;
					curSol = newSol->copyOfObj();
					curCriterion = newCriterion;					
				} else {
					double probOfChange = getRandom<double>(0.0, 1.0);
					if (probOfChange < exp(-diff / temp->getTemp()))
					{
				 		delete curSol;
				 		curSol = newSol->copyOfObj();
				 		curCriterion = newCriterion;					
					}	
				}	 
			}
			temp->decreaseTemp();
		}
		return bestSol;
	}
};