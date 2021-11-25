#pragma once
#include <cmath>

class temperature
{
private:
	double initTemp, curTemp;
	int iter;
	
public:
	temperature(double temp_ = 0) : initTemp(temp_), curTemp(temp_), iter(0) {}
	temperature * copyOfObj ()
	{
		return new temperature(initTemp);	
	}
		
	~temperature() = default;

	double getTemp ()
	{
		return curTemp;
	}


	void decreaseTemp()
	{
		iter++;
		curTemp = initTemp / log (1 + iter);
		return; 
	}
};
