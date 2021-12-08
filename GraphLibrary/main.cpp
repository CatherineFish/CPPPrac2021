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

	std::vector<char> completeParam = {'A', 'B', 'F'};

	std::vector<std::string> simpleParam = {{"EF"}, {"FA"}};

	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};

	auto bipartite = factory.Create("bipartite", std::make_unique<TBipartiteOptions>(first, second));
	std::cout << bipartite->ToString() << std::endl;
	// BipartiteGraph {{A, B, C, D}, {E, F}}
	auto complete = factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam));
	std::cout << complete->ToString() << std::endl;
	// CompleteGraph {A, B, F}
	auto simple = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam));
	std::cout << simple->ToString() << std::endl;
	//// SimpleGraph {EF, FA}
	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));
	std::cout << weighted->ToString() << std::endl;
	//WeightedGraph {FD:5, ED:6}
}