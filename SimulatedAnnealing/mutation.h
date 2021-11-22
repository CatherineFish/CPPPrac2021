#pragma once
#include "solution.h"

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
		std::cout << "numOfProc = " << numOfProc << " numOfjob = " << numOfjob << std::endl;
		double lastTStart = sol->getLastTStart(numOfProc, numOfjob); 
		size_t newNumOfProc = getRandom<int>(0, sol->getProcNum());
		
		oneJob* j = sol->eraseJob(numOfProc, numOfjob);
		size_t newNumOfjob = sol->emptyTask(newNumOfProc, lastTStart, j);		
		std::cout << "NEWnumOfProc = " << newNumOfProc << " NEWNumOfjob = " << newNumOfjob<< " lastTStart = " << lastTStart << std::endl;
		
		sol->insertJob(std::move(j), newNumOfProc, newNumOfjob);
		return sol;
	}
};
