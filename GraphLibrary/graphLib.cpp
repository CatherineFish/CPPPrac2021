#include <graphLib.h>

void TWeightedGraph::ToString() const override {
	std::cout << "WeightedGraph {";
	bool start = true;
	for (auto &i: vertexAndWeightedEdges->getEdgesWithWeights()) {
		if (!start) {
			std::cout << ", ";
		}
		start = false;
		std::cout << i.first.first << i.first.second << ":" << i.second;
	}
	std::cout << "}" << std::endl;
	return;
}


void TBipartiteGraph::ToString() const override {
	std::cout << "BipartiteGraph {{";
	bool start = true;
	for (auto &i: vertexVectors->getFirst()) {
		if (!start) {
			std::cout << ", ";
		}
		start = false;
		std::cout << i;
	}
	std::cout << "}, {";
	start = true;
	for (auto &i: vertexVectors->getSecond()) {
		if (!start) {
			std::cout << ", ";
		}
		start = false;
		std::cout << i;
	}
	std::cout << "}}" << std::endl;
	return;
}

void TCompleteGraph::ToString() const override {
	std::cout << "CompleteGraph {";
	bool start = true;
	for (auto &i: vertex->getVertex()) {
		if (!start) {
			std::cout << ", ";
		}
		start = false;
		std::cout << i;
	}
	std::cout << "}" << std::endl;
	return;
}


void TSimpleGraph::ToString() const override {
	std::cout << "SimpleGraph {";
	bool start = true;
	for (auto &i: edges->getEdges()) {
		if (!start) {
			std::cout << ", ";
		}
		start = false;
		std::cout << i.first.first << i.first.second;
	}
	std::cout << "}" << std::endl;
	return;
}
