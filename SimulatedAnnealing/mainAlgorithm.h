#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include <cstdlib>

#include "solution.h"
#include "mutation.h"
#include "temperature.h"

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