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
    TBipartiteOptions(std::vector<char> first, std::vector<char> second) {
        for (const auto& i: first) {
            if ('A' <= i && i <= 'Z') {
                firstGroupOfVertex.push_back(i);
            } else {
                throw std::invalid_argument("Vertex must be uppercase latin letter");
            }
        }

        std::sort(firstGroupOfVertex.begin(), firstGroupOfVertex.end());
        firstGroupOfVertex.erase(std::unique(firstGroupOfVertex.begin(), firstGroupOfVertex.end()), firstGroupOfVertex.end());
        for (const auto& i: second) {
            auto it = find(firstGroupOfVertex.begin(), firstGroupOfVertex.end(), i);
            if (it != firstGroupOfVertex.end()) {
                throw std::invalid_argument("Vertices in two parts must be different");
            }
            if ('A' <= i && i <= 'Z') {
                secondGroupOfVertex.push_back(i);
            } else {
                throw std::invalid_argument("Vertex must be uppercase latin letter");
            }
        }
        std::sort(secondGroupOfVertex.begin(), secondGroupOfVertex.end());
        secondGroupOfVertex.erase(std::unique(secondGroupOfVertex.begin(), secondGroupOfVertex.end()), secondGroupOfVertex.end());
        
        
    } 
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
        std::sort(result.begin(), result.end());
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
    
    TCompleteOptions(std::vector<char> vertex_) {
        for (const auto& i: vertex_) {
            if ('A' <= i && i <= 'Z') {
                allVertex.push_back(i);
            } else {
                throw std::invalid_argument("Vertex must be uppercase latin letter");
            }
        }
        std::sort(allVertex.begin(), allVertex.end());
        allVertex.erase(std::unique(allVertex.begin(), allVertex.end()), allVertex.end());
    }
    
    std::vector<char> getVertex() {
        return allVertex;
    }

    std::vector<std::pair<char, char>> getEdges() {
        std::vector<std::pair<char, char>> result;
        for (size_t i = 0; i < allVertex.size(); i++) {
            for (size_t j = i + 1; j < allVertex.size(); j++) {
                result.push_back({allVertex[i], allVertex[j]});
            }
        }
        return result;
    }


private:
    std::vector<char> allVertex;

};

class TSimpleOptions : public TOptions {
    
public:
    virtual ~TSimpleOptions() override = default;
    
    struct {
        bool operator()(std::pair<char, char> a, std::pair<char, char> b) const {
            if (a.first == b.first) {
                return a.second < b.second; 
            }
            return (a.first < b.first); 
        }
    } simpleCustomLess;

    TSimpleOptions(std::vector<std::string> edgesPairs)
    {
        for (const auto& i: edgesPairs) {
            if (i == "") continue;
            if (i.size() != 2) {
                throw std::invalid_argument("Edge name must be two letters length");
            }
            if (!('A' <= i[0] && i[0] <= 'Z' && 'A' <= i[1] && i[1] <= 'Z')) {
                throw std::invalid_argument("Edge name must be two uppercase latin letter");
            }
            if (i[0] < i[1]) {
                vertex.push_back(i[0]);
                vertex.push_back(i[1]);
                edges.push_back({i[0], i[1]});
            } else {
                vertex.push_back(i[1]);
                vertex.push_back(i[0]);
                edges.push_back({i[1], i[0]});
            }
            
        }
        std::sort(edges.begin(), edges.end(), simpleCustomLess);
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
    struct {
        bool operator()(std::pair<std::pair<char, char>, int> a, std::pair<std::pair<char, char>, int> b) const { 
            if (a.first.first == b.first.first) {
                return (a.first.second < b.first.second);
            }
            return a.first.first < b.first.first; 
        }
    } customLess;

    TWeightedOptions(std::vector<std::string> edgesPairs, std::vector<int> weights) {
        if (edgesPairs.size() != weights.size()) {
            throw std::invalid_argument("The number of edges and weights must be the same");
        }
        for (size_t i = 0; i < edgesPairs.size(); i++) {
            if (edgesPairs[i].size() != 2) {
                throw std::invalid_argument("Edge name must be two letters length");
            }
            if (!('A' <= edgesPairs[i][0] && edgesPairs[i][0] <= 'Z' && 'A' <= edgesPairs[i][1] && edgesPairs[i][1] <= 'Z')) {
                throw std::invalid_argument("Edge name must be two uppercase latin letter");
            }
            if (edgesPairs[i][0] < edgesPairs[i][1]) {
                vertex.push_back(edgesPairs[i][0]);
                vertex.push_back(edgesPairs[i][1]);
                edgesWithWeights.push_back({{edgesPairs[i][0], edgesPairs[i][1]}, weights[i]});
            } else {
                vertex.push_back(edgesPairs[i][1]);
                vertex.push_back(edgesPairs[i][0]);
                edgesWithWeights.push_back({{edgesPairs[i][1], edgesPairs[i][0]}, weights[i]});
            }
            

        }

        std::sort(edgesWithWeights.begin(), edgesWithWeights.end(), customLess);
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

