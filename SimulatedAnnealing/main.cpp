#include <iostream>
#include <vector>

#include "mainAlgorithm.h"
#include "solution.h"
#include "mutation.h"
#include "temperature.h"




int main()
{
		
	oneJob *j_0, *j_1, *j_2, *j_3, *j_4;
	j_0 = new oneJob(100.0, 0, {});
	j_1 = new oneJob(50.0, 1, {0, 2});
	j_2 = new oneJob(12.0, 2, {0});
	j_3 = new oneJob(8888.0, 3, {4});
	j_4 = new oneJob(70000.0, 4, {});
	
	j_0->initializeJob({});
	j_1->initializeJob({j_0, j_2});
	j_2->initializeJob({j_0});
	j_3->initializeJob({j_4});
	j_4->initializeJob({});
	std::vector<std::vector<oneJob *>> initSol;
	std::vector<oneJob *> first;
	first.push_back(j_0);
	first.push_back(j_2);
	first.push_back(j_1);
	first.push_back(j_4);
	first.push_back(j_3);
	initSol.push_back(first);
	initSol.push_back(std::vector<oneJob *>());
	initSol.push_back(std::vector<oneJob *>());
	solution *sol;
	double time = 0.0;
	for (size_t i = 0; i < first.size(); i++)
	{
		first[i]->setTStart(time);
		time += first[i]->getDuration();
	}
	sol = new solution(3, initSol);
	mutation * mut;
	mut = new mutation();
	mainAlgorithm alg(sol, 1000, mut);
	sol->print();
	solution *res = alg.mainCycle();
	std::cout << "RESULT" << std::endl;
	res->print();
}