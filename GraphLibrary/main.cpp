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

	auto bVertex = bipartite->GetVertices();
	for (auto elem: bVertex) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;

	auto bEdges = bipartite->GetEdges();
	for (auto elem: bEdges) {
		std::cout << elem.first << elem.second << " ";
	}
	std::cout << std::endl;

	
	auto complete = factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam));
	std::cout << complete->ToString() << std::endl;
	// CompleteGraph {A, B, F}

	auto cVertex = complete->GetVertices();
	for (auto elem: cVertex) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;	

	auto cEdges = complete->GetEdges();
	for (auto elem: cEdges) {
		std::cout << elem.first << elem.second << " ";
	}
	std::cout << std::endl;



	auto simple = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam));
	std::cout << simple->ToString() << std::endl;
	//// SimpleGraph {EF, FA}
	
	auto sVertex = simple->GetVertices();
	for (auto elem: sVertex) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;

	auto sEdges = simple->GetEdges();
	for (auto elem: sEdges) {
		std::cout << elem.first << elem.second << " ";
	}
	std::cout << std::endl;



	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));
	std::cout << weighted->ToString() << std::endl;
	//WeightedGraph {FD:5, ED:6}

	auto wVertex = weighted->GetVertices();
	for (auto elem: wVertex) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;

	auto wEdges = weighted->GetEdges();
	for (auto elem: wEdges) {
		std::cout << elem.first << elem.second << " ";
	}
	std::cout << std::endl;


}