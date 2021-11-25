#pragma once
#include "solution.h"

class mutation
{
public:
	~mutation() = default;

	solution* mutate(solution* sol)
	{
		int numOfProc = getRandom<int>(0, sol->getProcNum());
		while (sol->jobsNumOnProc(numOfProc) == 0)
		{
			numOfProc = getRandom<int>(0, sol->getProcNum());
		}
		size_t numOfjob = getRandom<int>(0, sol->jobsNumOnProc(numOfProc));
		int lastTStart = sol->getLastTStart(numOfProc, numOfjob); 
		size_t newNumOfProc = getRandom<int>(0, sol->getProcNum());
		oneJob* j = sol->eraseJob(numOfProc, numOfjob);
		size_t newNumOfjob = sol->emptyTask(newNumOfProc, lastTStart, j);		
		if (numOfProc == newNumOfProc && numOfjob == newNumOfjob) {
			sol->insertJob(std::move(j), newNumOfProc, newNumOfjob, false);	
		} else {
			sol->insertJob(std::move(j), newNumOfProc, newNumOfjob, true);
		}
		return sol;
	}

	mutation * copyOfObj() {
		return new mutation(*this);
	}
};
