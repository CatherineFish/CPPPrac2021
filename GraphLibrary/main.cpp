#include <cassert>
#include <iostream>
#include "factory.h"

int main() {
	TFactory factory;
	auto graphs = factory.GetAvailableGraphs();
	for (const auto& gen : graphs) {
		std::cout << gen << std::endl;
	}
	std::vector<char> first = {'A','B','C','D'};
	std::vector<char> second = {'E','F'};
	
	auto bipartite = factory.Create("bipartite", std::make_unique<TBipartiteOptions>(first, second));
	bipartite->ToString();
	// BipartiteGraph {{A, B, C, D}, {E, F}}
	//auto complete = factory.Create("complete", std::make_unique<TCompleteOptions>({'A', 'B', 'F'}));
	//std::cout << complete.ToString() << std::endl;
	//// CompleteGraph {A, B, F}
	//auto simple = factory.Create("simple", std::make_unique<TSimpleOptions>({'EF', 'FA'}));
	//std::cout << simple.ToString() << std::endl;
	//// SimpleGraph {EF, FA}
	//auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>({'FD', 'ED'}, {5, 6}));
	//std::cout << weighted.ToString() << std::endl;
	// WeightedGraph {FD:5, ED:6}
}