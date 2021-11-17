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
