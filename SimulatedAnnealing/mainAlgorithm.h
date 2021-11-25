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
 	int step;

public:
	mainAlgorithm(solution* initSol, temperature * initTemp, mutation* initMutation, solution * best = nullptr, int step_ = 10): step(step_){
		curMutation = initMutation->copyOfObj();
		curSol = initSol->copyOfObj();
		if (best == nullptr) {
			bestSol = initSol->copyOfObj();
			
		} else {
			bestSol = best->copyOfObj();
		}
		temp = initTemp->copyOfObj();
	} 
	
	~mainAlgorithm() {
		std::cout << "DELETER" << std::endl;
		delete(curSol);
		std::cout << "SOL DELETER END" << std::endl;
		
		delete(bestSol);
		std::cout << "BEST DELETER END" << std::endl;
		
		delete(temp);
		std::cout << "TEMP DELETER END" << std::endl;
		
		delete(curMutation);
		std::cout << "MUT DELETER END" << std::endl;
		
	}

	solution* mainCycle ()
	{
		//std::cout << "MAIN CYCLE\n";
		//std::string filename = "currentSolution.txt";
		//std::ofstream file;
  		//file.open(filename);
  		//int curStep = 0;

		int inMaxIter = globInMaxIter, outMaxIter = globOutMaxIter;

		curCriterion = curSol->getCriterion();
		
		bestCriterion = bestSol->getCriterion();
		
		//std::cout << "CUR CRIT: " << curCriterion << " GLOB CRIT: " << bestCriterion << std::endl;
		
		while (outMaxIter--)
		{
			while (inMaxIter--)
			{
				//std::cout << "out = " << outMaxIter << " in = " << inMaxIter << std::endl;
				solution * newSol = curMutation->mutate(curSol->copyOfObj());
				double newCriterion = newSol->getCriterion();
				
				//std::cout << "CUR SOLUTION:\n";
				//curSol->print();
		
				//std::cout << "NEW SOLUTION:\n";
				//newSol->print();
				//std::cout << "CUR CRITERION: " << curCriterion << std::endl;
				//std::cout << "NEW CRITERION: " << newCriterion << std::endl;
				
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
				/*curStep++;
				if (curStep == step) {
					file.open(filename, std::ios::app);
					file << "BEST SOLUTION" << std::endl;
					file.close();
					bestSol->printNumFile(filename);
					file.open(filename, std::ios::app);
  		
					file << "BEST CRITERION: " << bestCriterion << std::endl << std::endl;
					
					file << "CURRENT SOLUTION" << std::endl;
					file.close();
					curSol->printNumFile(filename);
					file.open(filename, std::ios::app);
  		
					file << "CURRENT CRITERION: " << curCriterion << std::endl << std::endl << std::endl << std::endl;
					file.close();
					curStep = 0;
				} */
					 
			}
			temp->decreaseTemp();
			inMaxIter = globInMaxIter;
		}
		return bestSol;
	}
};