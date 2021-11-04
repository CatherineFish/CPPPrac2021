#include <cmath>

class solution
{

};


class temperature
{
private:
	double initTemp, curTemp;
	
public:
	temperature(double temp_) : initTemp(temp_), curTemp(temp_) {}
	double getTemp ()
	{
		return curTemp;
	}
	void decreaseTemp(int iteration)
	{
		curTemp = initTemp / log (1 + iteration);
		return; 
	}
};


class mainAlgoritnhm
{

private:
	solution* curSol, bestSol;
	temperature* temp;
	//int outMaxIter, inMaxIter;
	int bestCriterion, curCriterion;
 

public:
	mainAlgoritnhm(solution sol_, temperature temp_) : curSol(sol_), temp(temp_), curCriterion(crit_) {}
	solution* mainCycle ()
	{
		bestSol = curSol;
		bestCriterion = curCriterion;
		int inMaxIter, outMaxIter;
		for (int i = 0; i < outMaxIter; i++)
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
			temp->decreaseTemp(i);
		}
		return bestSol;
	}
};