#include <memory>
#include <iostream>
#include "graphLib.h"

std::unique_ptr<TBipartiteGraph> operator+ (TBipartiteGraph &firstGraph, TBipartiteGraph &secondGraph) {
	std::vector<char> firstParam = firstGraph.vertexVectors->getFirst();
	std::vector<char> secondParam = firstGraph.vertexVectors->getSecond();
	for (const auto& elem: secondGraph.vertexVectors->getFirst()) {
		firstParam.push_back(elem);
	}

	for (const auto& elem: secondGraph.vertexVectors->getSecond()) {
		secondParam.push_back(elem);
	}

	return std::make_unique<TBipartiteGraph>(std::make_unique<TBipartiteOptions>(firstParam, secondParam));
}

std::unique_ptr<TCompleteGraph> operator+ (TCompleteGraph &firstGraph, TCompleteGraph &secondGraph) {
	std::vector<char> firstParam = firstGraph.vertex->getVertex();
	for (const auto& elem: secondGraph.vertex->getVertex()) {
		firstParam.push_back(elem);
	}

	return std::make_unique<TCompleteGraph>(std::make_unique<TCompleteOptions>(firstParam));
}

std::unique_ptr<TWeightedGraph> operator+ (TWeightedGraph &firstGraph, TWeightedGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.vertexAndWeightedEdges->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
        }
    std::vector<int> newWeights;
	for (const auto& i:firstGraph.vertexAndWeightedEdges->getEdgesWithWeights()) {
		newWeights.push_back(i.second);
	}

	std::vector<std::pair<std::pair<char, char>, int>> oldSecondWeights = secondGraph.vertexAndWeightedEdges->getEdgesWithWeights(); 
	std::vector<int> secondWeights;
	for (const auto& i: oldSecondWeights) {
		secondWeights.push_back(i.second);
	}
	for (const auto& elem: oldSecondWeights) {
		auto it = find(newEdges.begin(), newEdges.end(), std::string() + elem.first.first + elem.first.second);
  		if (it != newEdges.end()) {
  			int idx = it - newEdges.begin();
  			auto it_2 = find(oldSecondWeights.begin(), oldSecondWeights.end(), elem);
  			int idx_2 = it_2 - oldSecondWeights.begin();
  			if (secondWeights[idx_2] < newWeights[idx]) {
  				newWeights[idx] = secondWeights[idx_2];
  			}
  		} else {
  			newEdges.push_back(std::string() + elem.first.first + elem.first.second);
  			auto it_2 = find(oldSecondWeights.begin(), oldSecondWeights.end(), elem);
  			int idx_2 = it_2 - oldSecondWeights.begin();
  			newWeights.push_back(secondWeights[idx_2]);

  		}

	}
	return std::make_unique<TWeightedGraph>(std::make_unique<TWeightedOptions>(newEdges, newWeights));

}


std::unique_ptr<TWeightedGraph> operator+ (TWeightedGraph &firstGraph, TGraph &secondGraph) {
	throw std::invalid_argument("bad arguments");
}

std::unique_ptr<TCompleteGraph> operator+ (TCompleteGraph &firstGraph, TWeightedGraph &secondGraph) {
	throw std::invalid_argument("bad arguments");
}

std::unique_ptr<TBipartiteGraph> operator+ (TBipartiteGraph &firstGraph, TWeightedGraph &secondGraph) {
	throw std::invalid_argument("bad arguments");
}

std::unique_ptr<TSimpleGraph> operator+ (TSimpleGraph &firstGraph, TWeightedGraph &secondGraph) {
	throw std::invalid_argument("bad arguments");	
}


std::unique_ptr<TSimpleGraph> operator+ (TBipartiteGraph &firstGraph, TSimpleGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char, char>> firstEdges = firstGraph.vertexVectors->getEdges();
	std::vector<std::pair<char, char>> secondEdges = secondGraph.edges->getEdges();
	for (const auto& i: firstEdges) {
		newEdges.push_back(std::string() + i.first + i.second);
	}

	for (const auto& i: secondEdges) {
		newEdges.push_back(std::string() + i.first + i.second);
	}
	return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));
}

std::unique_ptr<TSimpleGraph> operator+ (TSimpleGraph &firstGraph, TBipartiteGraph &secondGraph) {
	return secondGraph + firstGraph;
}


std::unique_ptr<TSimpleGraph> operator+ (TBipartiteGraph &firstGraph, TCompleteGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char, char>> firstEdges = firstGraph.vertexVectors->getEdges();
	std::vector<std::pair<char, char>> secondEdges = secondGraph.vertex->getEdges();
	for (const auto& i: firstEdges) {
		newEdges.push_back(std::string() + i.first + i.second);
	}

	for (const auto& i: secondEdges) {
		newEdges.push_back(std::string() + i.first + i.second);
	}
	return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));
}

std::unique_ptr<TSimpleGraph> operator+ (TCompleteGraph &firstGraph, TBipartiteGraph &secondGraph) {
	return secondGraph + firstGraph;
}


std::unique_ptr<TSimpleGraph> operator+ (TCompleteGraph &firstGraph, TSimpleGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char, char>> firstEdges = firstGraph.vertex->getEdges();
	std::vector<std::pair<char, char>> secondEdges = secondGraph.edges->getEdges();
	for (const auto& i: firstEdges) {
		newEdges.push_back(std::string() + i.first + i.second);
	}

	for (const auto& i: secondEdges) {
		newEdges.push_back(std::string() + i.first + i.second);
	}
	return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));

}

std::unique_ptr<TSimpleGraph> operator+ (TSimpleGraph &firstGraph, TCompleteGraph &secondGraph) {
	return secondGraph + firstGraph;
}


std::unique_ptr<TSimpleGraph> operator+ (TSimpleGraph &firstGraph, TSimpleGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char, char>> firstEdges = firstGraph.edges->getEdges();
	std::vector<std::pair<char, char>> secondEdges = secondGraph.edges->getEdges();
	for (const auto& i: firstEdges) {
		newEdges.push_back(std::string() + i.first + i.second);
	}

	for (const auto& i: secondEdges) {
		newEdges.push_back(std::string() + i.first + i.second);
	}
	return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));

}


std::unique_ptr<TBipartiteGraph> operator- (TBipartiteGraph &firstGraph, TBipartiteGraph &secondGraph) {
	std::vector<char> firstParam = firstGraph.vertexVectors->getFirst();
	std::vector<char> secondParam = firstGraph.vertexVectors->getSecond();
	for (const auto& elem: secondGraph.vertexVectors->getFirst()) {
		remove(firstParam.begin(), firstParam.end(), elem);
		
	}

	for (const auto& elem: secondGraph.vertexVectors->getSecond()) {
		remove(secondParam.begin(), secondParam.end(), elem);
	}

	return std::make_unique<TBipartiteGraph>(std::make_unique<TBipartiteOptions>(firstParam, secondParam));
}

std::unique_ptr<TCompleteGraph> operator- (TCompleteGraph &firstGraph, TCompleteGraph &secondGraph) {
	std::vector<char> firstParam = firstGraph.vertex->getVertex();
	for (const auto& elem: secondGraph.vertex->getVertex()) {
		remove(firstParam.begin(), firstParam.end(), elem);
	}

	return std::make_unique<TCompleteGraph>(std::make_unique<TCompleteOptions>(firstParam));
}

std::unique_ptr<TWeightedGraph> operator- (TWeightedGraph &firstGraph, TWeightedGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.vertexAndWeightedEdges->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
    }
    std::vector<int> newWeights;
	for (const auto& i:firstGraph.vertexAndWeightedEdges->getEdgesWithWeights()) {
		newWeights.push_back(i.second);
	}


	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.vertexAndWeightedEdges->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	if (elem == "") continue;
    	auto it = find(newEdges.begin(), newEdges.end(), elem);
    	if (it != newEdges.end()) {
    		int idx_2 = it - newEdges.begin();
    		newEdges.erase(newEdges.begin() + idx_2);
			newWeights.erase(newWeights.begin() + idx_2);    		
    	}


    }
	return std::make_unique<TWeightedGraph>(std::make_unique<TWeightedOptions>(newEdges, newWeights));

}

std::unique_ptr<TWeightedGraph> operator- (TWeightedGraph &firstGraph, TBipartiteGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.vertexAndWeightedEdges->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
    }
    std::vector<int> newWeights;
	for (const auto& i:firstGraph.vertexAndWeightedEdges->getEdgesWithWeights()) {
		newWeights.push_back(i.second);
	}

	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.vertexVectors->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	auto it = find(newEdges.begin(), newEdges.end(), elem);
    	if (it != newEdges.end()) {
    		int idx_2 = it - secondNewEdges.begin();
    		remove(newEdges.begin(), newEdges.end(), elem);
			newWeights.erase(newWeights.begin() + idx_2);    		
    	}
    }
	return std::make_unique<TWeightedGraph>(std::make_unique<TWeightedOptions>(newEdges, newWeights));

}

std::unique_ptr<TWeightedGraph> operator- (TWeightedGraph &firstGraph, TSimpleGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.vertexAndWeightedEdges->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
    }
    std::vector<int> newWeights;
	for (const auto& i:firstGraph.vertexAndWeightedEdges->getEdgesWithWeights()) {
		newWeights.push_back(i.second);
	}

	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.edges->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	auto it = find(newEdges.begin(), newEdges.end(), elem);
    	if (it != newEdges.end()) {
    		int idx_2 = it - secondNewEdges.begin();
    		remove(newEdges.begin(), newEdges.end(), elem);
			newWeights.erase(newWeights.begin() + idx_2);    		
    	}
    }
	return std::make_unique<TWeightedGraph>(std::make_unique<TWeightedOptions>(newEdges, newWeights));

}

std::unique_ptr<TWeightedGraph> operator- (TWeightedGraph &firstGraph, TCompleteGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.vertexAndWeightedEdges->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
    }
    std::vector<int> newWeights;
	for (const auto& i:firstGraph.vertexAndWeightedEdges->getEdgesWithWeights()) {
		newWeights.push_back(i.second);
	}

	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.vertex->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	auto it = find(newEdges.begin(), newEdges.end(), elem);
    	if (it != newEdges.end()) {
    		int idx_2 = it - secondNewEdges.begin();
    		remove(newEdges.begin(), newEdges.end(), elem);
			newWeights.erase(newWeights.begin() + idx_2);    		
    	}
    }
	return std::make_unique<TWeightedGraph>(std::make_unique<TWeightedOptions>(newEdges, newWeights));

}


std::unique_ptr<TSimpleGraph> operator- (TSimpleGraph &firstGraph, TWeightedGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.edges->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
    }
    

	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.vertexAndWeightedEdges->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	remove(newEdges.begin(), newEdges.end(), elem);
    }
	return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));

}

std::unique_ptr<TSimpleGraph> operator- (TCompleteGraph &firstGraph, TWeightedGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.vertex->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
    }
    

	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.vertexAndWeightedEdges->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	remove(newEdges.begin(), newEdges.end(), elem);
    }
	return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));
}

std::unique_ptr<TSimpleGraph> operator- (TBipartiteGraph &firstGraph, TWeightedGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.vertexVectors->getEdges();
	for (const auto &i: oldEdges) {
        newEdges.push_back(std::string() + i.first + i.second);
    		
    }
    

	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.vertexAndWeightedEdges->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	remove(newEdges.begin(), newEdges.end(), elem);
    }
    return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));
}


std::unique_ptr<TSimpleGraph> operator- (TCompleteGraph &firstGraph, TSimpleGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.vertex->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
    }
    

	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.edges->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	remove(newEdges.begin(), newEdges.end(), elem);
    }
	return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));
}

std::unique_ptr<TSimpleGraph> operator- (TBipartiteGraph &firstGraph, TSimpleGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.vertexVectors->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
    }
    

	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.edges->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	remove(newEdges.begin(), newEdges.end(), elem);
    }
	return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));

}

std::unique_ptr<TSimpleGraph> operator- (TCompleteGraph &firstGraph, TBipartiteGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.vertex->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
    }
    

	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.vertexVectors->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	remove(newEdges.begin(), newEdges.end(), elem);
    }
	return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));

}

std::unique_ptr<TSimpleGraph> operator- (TBipartiteGraph &firstGraph, TCompleteGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.vertexVectors->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
    }
    

	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.vertex->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	remove(newEdges.begin(), newEdges.end(), elem);
    }
	return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));

}


std::unique_ptr<TSimpleGraph> operator- (TSimpleGraph &firstGraph, TSimpleGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.edges->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
    }
    

	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.edges->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	remove(newEdges.begin(), newEdges.end(), elem);
    }
	return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));

}

std::unique_ptr<TSimpleGraph> operator- (TSimpleGraph &firstGraph, TBipartiteGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.edges->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
    }
    

	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.vertexVectors->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	remove(newEdges.begin(), newEdges.end(), elem);
    }
	return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));

}

std::unique_ptr<TSimpleGraph> operator- (TSimpleGraph &firstGraph, TCompleteGraph &secondGraph) {
	std::vector<std::string> newEdges;
	std::vector<std::pair<char,char>> oldEdges = firstGraph.edges->getEdges();
	for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
    }
    

	std::vector<std::string> secondNewEdges;
	std::vector<std::pair<char,char>> secondEdges = secondGraph.vertex->getEdges();
	for (const auto &i: secondEdges) {
        secondNewEdges.push_back(std::string() + i.first + i.second);
    }

    for (const auto& elem: secondNewEdges) {
    	remove(newEdges.begin(), newEdges.end(), elem);
    }
	return std::make_unique<TSimpleGraph>(std::make_unique<TSimpleOptions>(newEdges));

}