#include <vector>
#include <algorithm>
#include <iostream>
#include <initializer_list>
#include <string>

class TOptions {
public:
    virtual ~TOptions() {}
};

class TBipartiteOptions : public TOptions {
    
public:
    virtual ~TBipartiteOptions() override = default;
    TBipartiteOptions(std::vector<char> first, std::vector<char> second): firstGroupOfVertex(first), secondGroupOfVertex(second) {}
    std::vector<char> getFirst() {
        return firstGroupOfVertex;
    }

    std::vector<char> getSecond() {
        return secondGroupOfVertex;
    }

    std::vector<char> getVertex() {
        std::vector <char> result = firstGroupOfVertex;
        for (auto&i : secondGroupOfVertex) {
            result.push_back(i);
        }
        return result;
    }

    std::vector<std::pair<char, char>> getEdges() {
        std::vector<std::pair<char, char>> result;
        for (const auto &first: firstGroupOfVertex) {
            for (const auto& second: secondGroupOfVertex) {
                result.push_back({first, second});
            }
        }
        return result;
    }

private:
    std::vector<char> firstGroupOfVertex;
    std::vector<char> secondGroupOfVertex;  
};

class TCompleteOptions : public TOptions {
    
public:
    virtual ~TCompleteOptions() override = default;
    
    TCompleteOptions(std::vector<char> vertex_): vertex(vertex_) {}
    
    std::vector<char> getVertex() {
        return vertex;
    }

    std::vector<std::pair<char, char>> getEdges() {
        std::vector<std::pair<char, char>> result;
        for (size_t i = 0; i < vertex.size(); i++) {
            for (size_t j = i + 1; j < vertex.size(); j++) {
                result.push_back({vertex[i], vertex[j]});
            }
        }
        return result;
    }


private:
    std::vector<char> vertex;

};

class TSimpleOptions : public TOptions {
    
public:
    virtual ~TSimpleOptions() override = default;
    
    TSimpleOptions(std::vector<std::string> edgesPairs)
    {
        for (const auto& i: edgesPairs) {
            vertex.push_back(i[0]);
            vertex.push_back(i[1]);
            edges.push_back({i[0], i[1]});
        }
        std::sort(vertex.begin(), vertex.end());
        vertex.erase(std::unique(vertex.begin(), vertex.end()), vertex.end());
    }

    std::vector<std::pair<char, char>> getEdges() {
        return edges;
    }

    std::vector<char> getVertex() {
        return vertex;
    }



private:
    std::vector<char> vertex;
    std::vector<std::pair<char, char>> edges;
};

class TWeightedOptions : public TOptions {
    
public:
    virtual ~TWeightedOptions() override{
    }
    
    TWeightedOptions(std::vector<std::string> edgesPairs, std::vector<int> weights) {
        for (size_t i = 0; i < edgesPairs.size(); i++) {
            vertex.push_back(edgesPairs[i][0]);
            vertex.push_back(edgesPairs[i][1]);
            edgesWithWeights.push_back({{edgesPairs[i][0], edgesPairs[i][1]}, weights[i]});

        }
        std::sort(vertex.begin(), vertex.end());
        vertex.erase(std::unique(vertex.begin(), vertex.end()), vertex.end());
    
    }

    std::vector<std::pair<std::pair<char, char>, int>> getEdgesWithWeights() {
        return edgesWithWeights;
    }

    std::vector<char> getVertex () {
        return vertex;
    }

    std::vector<std::pair<char, char>> getEdges() {
        std::vector<std::pair<char, char>> result;
        for (const auto &i:edgesWithWeights) {
            result.push_back(i.first);
        }
        return result;
    }
    
private:
    std::vector<char> vertex;
    std::vector<std::pair<std::pair<char, char>, int>> edgesWithWeights;
};

class TWeightedGraph;
class TSimpleGraph;
class TCompleteGraph;
class TBipartiteGraph;

class TGraph {
public:
    virtual const std::string ToString() const = 0;
    virtual const std::vector<char> GetVertices() const = 0;
    virtual const std::vector<std::pair<char, char>> GetEdges() const = 0;
    virtual std::unique_ptr<TWeightedGraph> AsWeighted(int defaultWeights) const = 0;

};

class TWeightedGraph : public TGraph{
public:
    using TOpt = TWeightedOptions;
    
    TWeightedGraph(std::unique_ptr<TOpt> initParams_) : vertexAndWeightedEdges(std::move(initParams_)){}
    
    virtual const std::string ToString() const override {
        std::string result =  "WeightedGraph {";
        bool start = true;
        for (auto &i: vertexAndWeightedEdges->getEdgesWithWeights()) {
            if (!start) {
                result += ", ";
            }
            start = false;
            result += i.first.first;
            result += i.first.second;
            result += ":";
            result += std::to_string(i.second);
        }
        result += "}";
        return result;
    }

    virtual const std::vector<char> GetVertices() const override {
        return vertexAndWeightedEdges->getVertex();
    }
    
    virtual const std::vector<std::pair<char, char>> GetEdges() const override {
        return vertexAndWeightedEdges->getEdges();
    }

    virtual std::unique_ptr<TWeightedGraph> AsWeighted(int defaultWeights) const override{
        std::vector<std::pair<char, char>> oldEdges = vertexAndWeightedEdges->getEdges();
        std::vector<std::string> newEdges; 
        std::vector<int> newWeights;
        for (const auto& i: vertexAndWeightedEdges->getEdgesWithWeights()) {
            newWeights.push_back(i.second);
        }
        for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
        }
        return std::make_unique<TWeightedGraph>(std::make_unique<TWeightedOptions>(newEdges, newWeights));
    }



    friend std::unique_ptr<TWeightedGraph> operator+ (TWeightedGraph &firstGraph, TWeightedGraph &secondGraph);
    friend std::unique_ptr<TWeightedGraph> operator+ (TWeightedGraph &firstGraph, TGraph &secondGraph);

    friend std::unique_ptr<TWeightedGraph> operator- (TWeightedGraph &firstGraph, TWeightedGraph &secondGraph);
    friend std::unique_ptr<TWeightedGraph> operator- (TWeightedGraph &firstGraph, TBipartiteGraph &secondGraph);
    friend std::unique_ptr<TWeightedGraph> operator- (TWeightedGraph &firstGraph, TSimpleGraph &secondGraph);
    friend std::unique_ptr<TWeightedGraph> operator- (TWeightedGraph &firstGraph, TCompleteGraph &secondGraph);
    std::unique_ptr<TOpt> vertexAndWeightedEdges;
    
private:
};


class TBipartiteGraph : public TGraph {
public:
    using TOpt = TBipartiteOptions; 
    
    TBipartiteGraph(std::unique_ptr<TOpt> initParams_) : vertexVectors(std::move(initParams_)) {}
    
    virtual const std::string ToString() const override {
        std::string result = "BipartiteGraph {{";
        bool start = true;
        for (auto &i: vertexVectors->getFirst()) {
            if (!start) {
                result += ", ";
            }
            start = false;
            result += i;
        }
        result += "}, {";
        start = true;
        for (auto &i: vertexVectors->getSecond()) {
            if (!start) {
                result += ", ";
            }
            start = false;
            result += i;
        }
        result += "}}";
        return result;
    
    }

    virtual const std::vector<char> GetVertices() const override {
        return vertexVectors->getVertex();
    }
    
    virtual const std::vector<std::pair<char, char>> GetEdges() const override {
        return vertexVectors->getEdges();
    }
    
    virtual std::unique_ptr<TWeightedGraph> AsWeighted(int defaultWeights) const override{
        std::vector<std::pair<char, char>> oldEdges = vertexVectors->getEdges();
        std::vector<std::string> newEdges; 
        std::vector<int> weights(oldEdges.size(), defaultWeights);
        for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
        }
        return std::make_unique<TWeightedGraph>(std::make_unique<TWeightedOptions>(newEdges, weights));
    }

    friend std::unique_ptr<TBipartiteGraph> operator+ (TBipartiteGraph &firstGraph, TBipartiteGraph &secondGraph);
    friend std::unique_ptr<TBipartiteGraph> operator+ (TBipartiteGraph &firstGraph, TWeightedGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator+ (TBipartiteGraph &firstGraph, TSimpleGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator+ (TBipartiteGraph &firstGraph, TCompleteGraph &secondGraph);
    
    friend std::unique_ptr<TBipartiteGraph> operator- (TBipartiteGraph &firstGraph, TBipartiteGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator- (TBipartiteGraph &firstGraph, TWeightedGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator- (TBipartiteGraph &firstGraph, TSimpleGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator- (TBipartiteGraph &firstGraph, TCompleteGraph &secondGraph);
    

    std::unique_ptr<TOpt> vertexVectors;
    
private:
};

class TCompleteGraph : public TGraph{
public:
    using TOpt = TCompleteOptions;
    
    TCompleteGraph(std::unique_ptr<TOpt> vertex_) : vertex(std::move(vertex_)) {}
    
    virtual const std::string ToString() const override{
        std::string result = "CompleteGraph {";
        bool start = true;
        for (auto &i: vertex->getVertex()) {
            if (!start) {
                result += ", ";
            }
            start = false;
            result += i;
        }
        result += "}";
        return result;
    }
    
    virtual const std::vector<char> GetVertices() const override{
        return vertex->getVertex();
    }

    virtual const std::vector<std::pair<char, char>> GetEdges() const override{
        return vertex->getEdges();
    }
    
    virtual std::unique_ptr<TWeightedGraph> AsWeighted(int defaultWeights) const override{
        std::vector<std::pair<char, char>> oldEdges = vertex->getEdges();
        std::vector<std::string> newEdges; 
        std::vector<int> weights(oldEdges.size(), defaultWeights);
        for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
        }
        return std::make_unique<TWeightedGraph>(std::make_unique<TWeightedOptions>(newEdges, weights));
    }

    friend std::unique_ptr<TCompleteGraph> operator+ (TCompleteGraph &firstGraph, TCompleteGraph &secondGraph);
    friend std::unique_ptr<TCompleteGraph> operator+ (TCompleteGraph &firstGraph, TWeightedGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator+ (TCompleteGraph &firstGraph, TBipartiteGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator+ (TCompleteGraph &firstGraph, TSimpleGraph &secondGraph);
    
    friend std::unique_ptr<TCompleteGraph> operator- (TCompleteGraph &firstGraph, TCompleteGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator- (TCompleteGraph &firstGraph, TWeightedGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator- (TCompleteGraph &firstGraph, TBipartiteGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator- (TCompleteGraph &firstGraph, TSimpleGraph &secondGraph);
    

    std::unique_ptr<TOpt> vertex;


private:
};

class TSimpleGraph : public TGraph{
public:
    using TOpt = TSimpleOptions;
    
    TSimpleGraph(std::unique_ptr<TOpt> edges_) : edges(std::move(edges_)) {}
    
    virtual const std::string ToString() const override{
        std::string result = "SimpleGraph {";
        bool start = true;
        for (auto &i: edges->getEdges()) {
            if (!start) {
                result += ", ";
            }
            start = false;
            result += i.first;
            result += i.second;
        }
        result += "}";
        return result;
    }
    
    virtual const std::vector<char> GetVertices() const override {
        return edges->getVertex();
    }

    virtual const std::vector<std::pair<char, char>> GetEdges() const override{
        return edges->getEdges();
    }
    
    virtual std::unique_ptr<TWeightedGraph> AsWeighted(int defaultWeights) const override{
        std::vector<std::pair<char, char>> oldEdges = edges->getEdges();
        std::vector<std::string> newEdges; 
        std::vector<int> weights(oldEdges.size(), defaultWeights);
        for (const auto &i: oldEdges) {
            newEdges.push_back(std::string() + i.first + i.second);
        }
        return std::make_unique<TWeightedGraph>(std::make_unique<TWeightedOptions>(newEdges, weights));
    }

    friend std::unique_ptr<TSimpleGraph> operator+ (TSimpleGraph &firstGraph, TWeightedGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator+ (TSimpleGraph &firstGraph, TBipartiteGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator+ (TSimpleGraph &firstGraph, TCompleteGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator+ (TSimpleGraph &firstGraph, TSimpleGraph &secondGraph);
    
    friend std::unique_ptr<TSimpleGraph> operator- (TSimpleGraph &firstGraph, TWeightedGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator- (TSimpleGraph &firstGraph, TBipartiteGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator- (TSimpleGraph &firstGraph, TCompleteGraph &secondGraph);
    friend std::unique_ptr<TSimpleGraph> operator- (TSimpleGraph &firstGraph, TSimpleGraph &secondGraph);
    
    std::unique_ptr<TOpt> edges;
    
    
private:
    
};

