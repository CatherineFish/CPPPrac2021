#include <iostream>
#include <cstdlib>
#include <fstream>
#include <utility>

#include "solution.h"

int distForWeight(int min, int max, int disp){
	std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<int> dist{(max - min) / 2, disp}; 
    int res = dist(gen);
    while (res < min || res > max) {
    	res = dist(gen);
    }
    return res;
}

int distForDegree(int mean, int disp, int vertexNum){
	std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<int> dist{mean, disp}; 
    int res = dist(gen);
    while (res < 0 || res > vertexNum) {
    	res = dist(gen);
    }
    return res;
}

void compInitialize (std::vector<std::pair<oneJob *, int>>  *allJobs, int vertexNum, int compNum, int dispComp)
{
	int vertex = vertexNum, newCompVertexNum, j = 0;
	while (vertex > compNum) {
		newCompVertexNum = distForDegree(compNum, dispComp, vertex);
		for (size_t i = vertexNum - vertex - newCompVertexNum; i < vertexNum - vertex; i++) {
			*allJobs[i].second = j;
		}
		j++;
		vertex -= newCompVertexNum;
	}
	for (size_t i = vertexNum - vertex; i < vertexNum; i++) {
		*allJobs[i].second = j;
	}
	return;
}

void degreeInitialize (std::vector<std::pair<oneJob *, int>>  *allJobs, int vertexDegree, int dispDegree)
{
	int j = 0, cur = 0, k = 0, last = 0;
	while (true) {
		k = 0;
		for (size_t i = cur; i < allJobs.size(); i++) {
			if (allJobs[i].second == j) {
				cur++;
				k++;
			} else {
				break;
			}
		}
		for (size_t i = last; i < cur; i++) {
			allJobs[i].first->depNum = std::vector(distForDegree(vertexDegree, dispDegree, k - (cur - last)), -1);
			for (size_t ii = 0; ii < allJobs[i].first->depNum.size(); ii++) {
				allJobs[i].first->depNum.push_back(allJobs[last + i + ii].first);
			}
		}
		j++;
		last = cur;
	}
	return;
}

void inputFormat (std::vector<oneJob *> allJobs, std::string filename) {
	std::ofstream file;
  	file.open(filename + ".txt");
  	file << allJobs.size() << std::endl;
  	
  	for (auto job : allJobs) {
  		file << job->getDuration();
  		for (auto j: job->depNum) {
  			file << " " << j;
  		}
  		file << std::endl;
  	}
  	return;
}

void dotFormat (std::vector<oneJob *> allJobs, std::string filename) {
	std::ofstream file;
  	file.open(filename + ".txt");
  	file << "digraph G {" << std::endl;
  	file << "    graph [fontsize=24]" << std::endl;
  	file << "    edge [fontsize=24]" << std::endl;
 	file << "    node [fontsize=24]" << std::endl;
  	file << "    ranksep = 1.5" << std::endl;
  	file << "    nodesep = .25" << std::endl;
  	file << "    edge [style="setlinewidth(3)"];" << std::endl;
  	for (auto job : allJobs) {
  		file << "    " << job->getDuration() << " -> { ";
  		for (auto j: job->depNum) {
  			file << j << " ";
  		}
  		file << "}" << std::endl;
  		
  	}
  	file << "}" << std::endl;
}

int main (int argc, char * argv[]) 
{
	if (argc < 9){
		std::cout <<"Incorrect arguments" << std::endl;
		return 0;
	}
	int vertexNum = std::atoi(argv[1]);
	int minWeight = std::atoi(argv[2]), maxWeight = std::atoi(argv[3]);
	int dispVertexWeight = std::atoi(argv[4]);
	int compNum = std::atoi(argv[5]), dispComp = std::atoi(argv[6]);
	int vertexDegree = std::atoi(argv[7]), dispDegree = std::atoi(argv[8]);

	std::vector<std::pair<std::oneJob *, int>> jobVector;
	for (size_t i = 0; i < vertexNum; i++) {
		jobVector.push_back(std::make_pair(new oneJob (distForWeight(minWeight, maxWeight, distForWeight), i), -1));
		//jobVector[i].first->depNum = std::vector<size_t>(distForDegree(vertexDegree, dispDegree, vertexNum), -1);
	}
	compInitialize(&allJobs,  vertexNum, compNum, dispComp);
	degreeInitialize(&allJobs, vertexDegree, distForDegree);
	
}