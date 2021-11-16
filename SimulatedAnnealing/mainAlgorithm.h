#include <cmath>
#include <vector>

class oneJob
{
private:
	double duration;

public:
	oneJob (dur_): duration(dur_){}

	double getDuration ()
	{
		return duration;
	}
};

class solution
{
	// вектор векторов
private:
	int procNum;
	std::vector<std::vector<oneJob>> sol;
public:
	solution()
	{
		procNum = 0;
		sol = std::vector<std::vector<oneJob>> (); 
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
	int bestCriterion, curCriterion;
 

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
		while (outMaxIter--)
		{
			while (inMaxIter--)
			{
				solution * newSol = curMutation->mutate(curSol->copyOfObj());
				int newCriterion = newSol->getCriterion();
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