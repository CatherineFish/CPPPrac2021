#include <memory>
#include <map>
#include <bits/stdc++.h>
#include "graphLib.h"
#include "findFunction.h"


std::vector<std::pair<char, char>> findFunction (TWeightedGraph &graph, char start, char finish)
{
    auto vertex = graph.GetVertices();
    auto edges = graph.GetEdges();
    auto edgesAndWeights = graph.vertexAndWeightedEdges->getEdgesWithWeights();
    std::vector<int> weights;
    for (const auto &elem: edgesAndWeights) {
        weights.push_back(elem.second);
    }
    std::vector<std::pair<char, char>> path;
    std::vector<char> visited;
    std::map<char, int> pathLenght;
    for (const auto &v: vertex) {
        if (v == start) {
            pathLenght[v] = 0;
        } else {
            pathLenght[v] = INT_MAX;
        }
    }
    visited.push_back(start);
    char cur = start;
    while (visited.size() != vertex.size()) {
        int min_path = INT_MAX;
        char candidate = cur;
        auto len = edges.size();
        for (int i = 0; i < len; ++i) {
            if (edges[i].first == cur) {
                auto place = std::find(visited.begin(), visited.end(), edges[i].second);
                if (place == visited.end()) {
                    if (pathLenght[cur] + weights[i] < pathLenght[edges[i].second]) {
                        pathLenght[edges[i].second] = pathLenght[cur] + weights[i];
                    }
                    int cur_path = weights[i];
                    if (cur_path < min_path) {
                        candidate = edges[i].second;
                        min_path = cur_path;
                    }
                }
            }
            if (edges[i].second == cur) {
                auto place = std::find(visited.begin(), visited.end(), edges[i].first);
                if (place == visited.end()) {
                    if (pathLenght[cur] + weights[i] < pathLenght[edges[i].first]) {
                        pathLenght[edges[i].first] = pathLenght[cur] + weights[i];
                    }
                    int cur_path = weights[i];
                    if (cur_path < min_path) {
                        candidate = edges[i].first;
                        min_path = cur_path;
                    } 
                }
            }
        }
        if (candidate == cur && visited.size() != vertex.size()) {
            throw std::logic_error("Wrong input");
        }
        visited.push_back(candidate);
        cur = candidate;
    }
    auto cur_length = pathLenght[finish];
    char next;
    cur = finish;
    while (cur != start) {
        auto len = edges.size();
        for (int i = 0; i < len; ++i) {
            if (edges[i].first == cur) {
                if (pathLenght[cur] - weights[i] == pathLenght[edges[i].second]) {
                    next = edges[i].second;
                    break;
                }
            }
            if (edges[i].second == cur) {
                if (pathLenght[cur] - weights[i] == pathLenght[edges[i].first]) {
                    next = edges[i].first;
                    break;
                }
            }
        }
        path.push_back(std::make_pair(next, cur));
        cur = next;
    }
    std::reverse(path.begin(), path.end());
    return path;
}