class solution
{

};


class temperature
{
private:
	double temp;
	
public:
	temperature(double temp_) : temp(temp_) {}
};


class mainAlgoritnhm
{

private:
	solution* curSol, bestSol;
	temperature* temp;
	int outMaxIter, inMaxIter;
	int bestCriterion, curCriterion;
 

public:
	mainAlgoritnhm(solution sol_, temperature temp_) : sol(sol_), temp(temp_) {}
	solution* mainCycle ()
	{
		while (outMaxIter--)
		{
			while (inMaxIter--)
			{
				solution newSol;
				int newCriterion;
				// сначала проверить, лучше ли лучшего
				// потом сравниваем с текущем
				if (curCriterion < criterion)
				{
					bestSol = newSol;
					criterion = curCriterion;
				} else {
					double probOfChange;
					double random;
					if (random < probOfChange)
					{
						curSol = newSol;
						curCriterion = newCriterion;
					}
				} 
			}
			temp->decreaseTemp();
		}
		return bestSol;
	}
};