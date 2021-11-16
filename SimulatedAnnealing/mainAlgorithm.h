#include <cmath>
#include <vector>

class oneJob
{
private:
	double duration;
	std::vector<int> dependences;
	int num;
	
public:
	oneJob (double dur_, std::vector<int> dep_, int num_): duration(dur_), dependences(dep_), num(num_){}

	double getDuration ()
	{
		return duration;
	}

	int getNum ()
	{
		return num;
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
		double Tmax = 0.0, Tmin = -1.0, Tcur;
		for (size_t i = 0; i < procNum; i++)
		{
			Tcur = 0.0;
			for (auto j: sol[i])
			{
				Tcur += j.getDuration();
			}
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