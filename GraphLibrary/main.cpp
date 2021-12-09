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

	auto Bweighted = bipartite->AsWeighted(5);

	std::cout << Bweighted->ToString() << std::endl;


	std::vector<char> first_2 = {'G', 'H', 'M'};
	std::vector<char> second_2 = {'K', 'L', 'Z'};

	std::cout << "SUM\n";

	auto bipartite_2 = factory.Create("bipartite", std::make_unique<TBipartiteOptions>(first_2, second_2));
	auto bipartite_sum = *(dynamic_cast<TBipartiteGraph*>(bipartite.get())) + *(dynamic_cast<TBipartiteGraph*>(bipartite_2.get()));
	std::cout << bipartite_sum->ToString() << std::endl;
	

	
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


	auto Cweighted = complete->AsWeighted(5);

	std::cout << Cweighted->ToString() << std::endl;

	std::vector<char> completeParam_2 = {'C', 'D'};

	std::cout << "SUM\n";

	auto complete_2 = factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam_2));
	auto complete_sum = *(dynamic_cast<TCompleteGraph*>(complete.get())) + *(dynamic_cast<TCompleteGraph*>(complete_2.get()));
	std::cout << complete_sum->ToString() << std::endl;



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


	auto Sweighted = simple->AsWeighted(5);

	std::cout << Sweighted->ToString() << std::endl;



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

	auto Wweighted = weighted->AsWeighted(5);

	std::cout << Wweighted->ToString() << std::endl;


	std::vector<std::string> weightedParamEdges_2 = {{"FD"}, {"ED"}, {"MP"}};
	std::vector<int> weightedParamW_2 = {3, 6, 7};
	std::cout << "SUM\n";

	auto weighted_2 = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges_2, weightedParamW_2));
	
	auto weighted_sum = *(dynamic_cast<TWeightedGraph*>(weighted.get())) + *(dynamic_cast<TWeightedGraph*>(weighted_2.get()));
	std::cout << weighted_sum->ToString() << std::endl;



}