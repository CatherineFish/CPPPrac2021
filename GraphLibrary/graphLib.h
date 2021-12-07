#include <vector>
class TOptions {
public:
    //virtual ~TOptions();
    virtual bool IsValid() const = 0;
};

class TBipartiteOptions : public TOptions {
    
public:

    TBipartiteOptions(std::vector<char> first, std::vector<char> second): firstGroupOfVertex(first), secondGroupOfVertex(second) {}
    
private:
    std::vector<char> firstGroupOfVertex;
    std::vector<char> secondGroupOfVertex;  
};

class TCompleteOptions : public TOptions {
    
public:
    TCompleteOptions(std::vector<char> vertex_): vertex(vertex_) {}
    
private:
    std::vector<char> vertex;

};

class TSimpleOptions : public TOptions {
    
public:

    TSimpleOptions(std::vector<char*> edgesPairs)
    {
        for (const auto& i: edgesPairs) {
            vertex.push_back(i[0]);
            vertex.push_back(i[1]);
            edges.push_back({i[0], i[1]});
        }
        sort(vertex.begin(), vertex.end());
        vertex.erase(unique(vertex.begin(), vertex.end()), vertex.end());
    }

private:
    std::vector<char> vertex;
    std::vector<std::pair<char, char>> edges;
};

class TWeightedOptions : public TOptions {
    
public:
    
    TWeightedOptions(std::vector<char*> edgesPairs, std::vector<int> weights) {
        for (size_t i = 0; i < edgesPairs.size(); i++) {
            vertex.push_back(edgesPairs[i][0]);
            vertex.push_back(edgesPairs[i][1]);
            edges.push_back({{edgesPairs[i][0], edgesPairs[i][1]}, weights[i]});

        }
        sort(vertex.begin(), vertex.end());
        vertex.erase(unique(vertex.begin(), vertex.end()), vertex.end());
    
    }
    
private:
    std::vector<char> vertex;
    std::vector<std::pair<std::pair<char, char>, int>> edgesWithWeights;
};

class TGraph {
public:
    virtual void ToString() const = 0;
    virtual const std::vector<char> GetVertices() const = 0;
    virtual const std::vector<std::vector<char>> GetEdges() const = 0;
    virtual TGraph AsWeighted(int defaultWeights) const = 0;

};

class TWeightedGraph : public TGraph{
public:
    using TOpt = TWeightedOptions;
    TWeightedGraph(std::unique_ptr<TOpt> initParams_) : vertexAndWeightedEdges(std::move(initParams_)){}
    virtual void ToString() const override;
    virtual const std::vector<char> GetVertices() const override;
    virtual const std::vector<std::vector<char>> GetEdges() const override;
    
private:
    std::unique_ptr<TOpt> vertexAndWeightedEdges;
};


class TBipartiteGraph : public TGraph {
public:
    using TOpt = TBipartiteOptions; 
    TBipartiteGraph(std::unique_ptr<TOpt> initParams_) : vertexVectors(std::move(initParams_)) {}
    virtual void ToString() const override{}
    virtual const std::vector<char> GetVertices() const override;
    virtual const std::vector<std::vector<char>> GetEdges() const override{}
    virtual TGraph AsWeighted(int defaultWeights) const override;
private:
    std::unique_ptr<TOpt> vertexVectors;
};

class TCompleteGraph : public TGraph{
public:
    using TOpt = TCompleteOptions;
    TCompleteGraph(std::unique_ptr<TOpt> vertex_) : vertex(std::move(vertex_)) {}
    virtual void ToString() const override{}
    virtual const std::vector<char> GetVertices() const override;
    virtual const std::vector<std::vector<char>> GetEdges() const override{}
    virtual TGraph AsWeighted(int defaultWeights) const override;
private:
    std::unique_ptr<TOpt> vertex;
};

class TSimpleGraph : public TGraph{
public:
    using TOpt = TSimpleOptions;
    TSimpleGraph(std::unique_ptr<TOpt> edges_) : edges(std::move(edges_)) {}
    virtual void ToString() const override{}
    virtual const std::vector<char> GetVertices() const override;
    virtual const std::vector<std::vector<char>> GetEdges() const override{}
    virtual TGraph AsWeighted(int defaultWeights) const override;
private:
    std::unique_ptr<TOpt> edges;
    
};

