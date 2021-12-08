#include <vector>
#include <algorithm>
#include <iostream>

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
private:
    std::vector<char> vertex;

};

class TSimpleOptions : public TOptions {
    
public:
    virtual ~TSimpleOptions() override = default;
    
    TSimpleOptions(std::vector<char*> edgesPairs)
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

private:
    std::vector<char> vertex;
    std::vector<std::pair<char, char>> edges;
};

class TWeightedOptions : public TOptions {
    
public:
    virtual ~TWeightedOptions() override{
        std::cout << "destr" << std::endl;
    }
    
    TWeightedOptions(std::vector<char*> edgesPairs, std::vector<int> weights) {
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
    
private:
    std::vector<char> vertex;
    std::vector<std::pair<std::pair<char, char>, int>> edgesWithWeights;
};

class TWeightedGraph;

class TGraph {
public:
    virtual void ToString() const = 0;
    //virtual const std::vector<char> GetVertices() const = 0;
    //virtual const std::vector<std::vector<char>> GetEdges() const = 0;
    //virtual std::unique_ptr<TWeightedGraph> AsWeighted(int defaultWeights) const = 0;

};

class TWeightedGraph : public TGraph{
public:
    using TOpt = TWeightedOptions;
    TWeightedGraph(std::unique_ptr<TOpt> initParams_) : vertexAndWeightedEdges(std::move(initParams_)){}
    virtual void ToString() const override {
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
    //virtual const std::vector<char> GetVertices() const override;
    //virtual const std::vector<std::vector<char>> GetEdges() const override;
    
private:
    std::unique_ptr<TOpt> vertexAndWeightedEdges;
};


class TBipartiteGraph : public TGraph {
public:
    using TOpt = TBipartiteOptions; 
    TBipartiteGraph(std::unique_ptr<TOpt> initParams_) : vertexVectors(std::move(initParams_)) {}
    virtual void ToString() const override {
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
    //virtual const std::vector<char> GetVertices() const override;
    //virtual const std::vector<std::vector<char>> GetEdges() const override{}
    //virtual std::unique_ptr<TWeightedGraph> AsWeighted(int defaultWeights) const override;
private:
    std::unique_ptr<TOpt> vertexVectors;
};

class TCompleteGraph : public TGraph{
public:
    using TOpt = TCompleteOptions;
    TCompleteGraph(std::unique_ptr<TOpt> vertex_) : vertex(std::move(vertex_)) {}
    virtual void ToString() const override{
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
    //virtual const std::vector<char> GetVertices() const override;
    //virtual const std::vector<std::vector<char>> GetEdges() const override{}
    //virtual std::unique_ptr<TWeightedGraph> AsWeighted(int defaultWeights) const override;
private:
    std::unique_ptr<TOpt> vertex;
};

class TSimpleGraph : public TGraph{
public:
    using TOpt = TSimpleOptions;
    TSimpleGraph(std::unique_ptr<TOpt> edges_) : edges(std::move(edges_)) {}
    virtual void ToString() const override{
        std::cout << "SimpleGraph {";
        bool start = true;
        for (auto &i: edges->getEdges()) {
            if (!start) {
                std::cout << ", ";
            }
            start = false;
            std::cout << i.first << i.second;
        }
        std::cout << "}" << std::endl;
        return;
    }
    //virtual const std::vector<char> GetVertices() const override;
    //virtual const std::vector<std::vector<char>> GetEdges() const override{}
    //virtual std::unique_ptr<TWeightedGraph> AsWeighted(int defaultWeights) const override;
private:
    std::unique_ptr<TOpt> edges;
    
};

