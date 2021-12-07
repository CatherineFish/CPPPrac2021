#include <cassert>
#include <iostream>
#include "factory.h"

int main() {
	TFactory factory;
	auto graphs = factory.GetAvailableGraphs();
	for (const auto& gen : graphs) {
		std::cout << gen << std::endl;
	}
}