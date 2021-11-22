#pragma once
#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include <cstdlib>

#include "solution.h"
#include "mutation.h"
#include "temperature.h"

class mainAlgorithm
{

private:
	solution* curSol, *bestSol;
	temperature* temp;
	mutation* curMutation;
	int globOutMaxIter = 1, globInMaxIter = 1;
	double bestCriterion, curCriterion;
 

public:
	mainAlgorithm(solution* initSol, double initTemp, mutation* initMutation): curMutation(initMutation){
		curSol = initSol->copyOfObj();
		bestSol = initSol->copyOfObj();
		temp = new temperature(initTemp);
	} 
	
	~mainAlgorithm() {
		delete(curSol);
		delete(bestSol);
		delete(temp);
		delete(curMutation);
	}

	solution* mainCycle ()
	{
		std::cout << "MAIN CYCLE\n";
		int inMaxIter = globInMaxIter, outMaxIter = globOutMaxIter;
		curCriterion = curSol->getCriterion();
		bestCriterion = bestSol->getCriterion();
		std::cout << "CUR CRIT: " << curCriterion << " GLOB CRIT: " << bestCriterion << std::endl;
		while (outMaxIter--)
		{
			while (inMaxIter--)
			{

				std::cout << "out = " << outMaxIter << " in = " << inMaxIter << std::endl;
				solution * newSol = curMutation->mutate(curSol->copyOfObj());
				double newCriterion = newSol->getCriterion();
				
				std::cout << "CUR SOLUTION:\n";
				curSol->print();
		
				std::cout << "NEW SOLUTION:\n";
				newSol->print();
				std::cout << "CUR CRITERION: " << curCriterion << std::endl;
				std::cout << "NEW CRITERION: " << newCriterion << std::endl;
				
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
			inMaxIter = globInMaxIter;
		}
		return bestSol;
	}
};