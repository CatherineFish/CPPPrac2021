#include <cmath>
#include <vector>

class oneJob
{
private:
	double duration, tStart = 0.0;
	std::vector<oneJob *> dependences;
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
			time = std::max(time, i->getTStart + i->getDuration);
		}
		return time;
	}
};

class solution
{

private:
	int procNum;
	std::vector<std::vector<oneJob>> sol;
public:
	solution(int procNum_, std::vector<std::vector<oneJob>> sol_): procNum(procNum_), sol(std::move(sol_)) {}
	
	~solution() = default;

	double getCriterion()
	{
		double Tmax = 0.0, Tmin = -1.0, Tcur, tLast = 0.0;
		for (size_t i = 0; i < procNum; i++)
		{
			Tcur = ((sol[i])[sol[i].size() - 1]).getTStart() + ((sol[i])[sol[i].size() - 1]).getDuration();
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

	std::vector<int> emptyTask(int procNum_, double duration, double tStart)
	{
		double lastTime = 0.0;
		std::vector<int> forInsert = {};
		for (size_t i = 0; i < sol[procNum_].size(); i++)
		{
			if (sol[procNum_][i].getTStart() > tStart)
			{
				if (sol[procNum_][i].getTStart() - tStart < duration)
				{
					forInsert.push_back(i);
				}
			}
			lastTime = sol[procNum_][i].getTStart() + sol[procNum_][i].getDuration(); 
		}
		return forInsert;
	}

};

class mutation
{
public:
	mutation* copy() = 0;
    ~mutation() = default;

	solution* mutate(solution* sol)
	{
		size_t numOfProc = getRandom<int>(0, sol->getProcNum());
		while (sol->jobsNumOnProc(numOfProc) == 0)
		{
			numOfProc = getRandom<int>(0, sol->getProcNum());
		}
		size_t numOfjob = getRandom<int>(0, sol->jobsNumOnProc);
		double lastTStart = (sol[numOfProc])[numOfjob].getLastTStart(); 
		size_t newNumOfProc = getRandom<int>(0, sol->getProcNum());
		while (sol->jobsNumOnProc(newnumOfProc) == 0)
		{
			newnumOfProc = getRandom<int>(0, sol->getProcNum());
		}

		std::vector<int> forInsert = sol->emptyTask(newNumOfProc, sol[numOfProc][numOfjob].getDuration(), lastTStart);
		size_t newNumOfjob = getRandom<int>(0, forInsert.size());
		oneJob j = sol->eraseJob(numOfProc, numOfjob);
		sol->insertJob(std::move(j), newNumOfProc, forInsert[newNumOfjob]);
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


	void decreaseTemp(int iteration)
	{
		iter++;
		curTemp = initTemp / log (1 + iter);
		return; 
	}
};



class mainAlgoritnhm
{

private:
	solution* curSol, bestSol;
	temperature* temp;
	mutation* curMutation;
	int globOutMaxIter = 100, globInMaxIter = 20;
	double bestCriterion, curCriterion;
 

public:
	mainAlgoritnhm(solution* initSol, double initTemp, mutation* initMutation): curMutation(initMutation){
		curSol = initSol->copyOfObj();
		best = initSol->copyOfObj();
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