#include <map>
#include <stdexcept>
#include "factory.h"

class TFactory::TImpl {
	class ICreator {
	public:
		virtual ~ICreator(){}
		virtual std::unique_ptr<TGraph> Create(std::unique_ptr<TOptions> &&opts) const = 0;
	};

	using TCreatorPtr = std::shared_ptr<ICreator>;
	using TRegisteredCreators = std::map<std::string, TCreatorPtr>;
	TRegisteredCreators RegisteredCreators;

public:
	template <class TCurrentGraph>
	class TCreator : public ICreator {
		std::unique_ptr<TGraph> Create(std::unique_ptr<TOptions> &&opts) const override{
			//try here
            auto CurrenOptions = dynamic_cast<typename TCurrentGraph::TOpt*>(opts.get());
            opts.release();
            if (!CurrenOptions || !CurrenOptions->IsValid())
            {
                throw std::invalid_argument();
            }
            return std::make_unique<TCurrentGraph>(std::unique_ptr<typename TCurrentGraph::TOpt>(CurrenOptions));
		}
	};
    TImpl() { 
        RegisterAll();
    }
    
    template <typename T>
    void RegisterCreator(const std::string& type) {
        RegisteredCreators[type] = std::make_shared<TCreator<T>>();
    }
    
    void RegisterAll() {
        RegisterCreator<TBipartiteGraph>("bipartite");
        RegisterCreator<TCompleteGraph>("complete");
        RegisterCreator<TSimpleGraph>("simple");
        RegisterCreator<TWeightedGraph>("weighted");    
    }

    std::unique_ptr<TGraph> CreateGraph(const std::string& type std::unique_ptr<TOptions> &&opts) const {
        auto creator = RegisteredCreators.find(t);
        if (creator == RegisteredCreators.end()) {
            throw std::invalid_argument();
        }
        return creator->second->Create(std::move(opts));
    }

    std::vector<std::string> GetAvailableGraphs () const {
        std::vector<std::string> result;
        for (const auto& creatorPair : RegisteredCreators) {
            result.push_back(creatorPair.first);
        }
        return result;
    }
};


std::unique_ptr<TGraph> TFactory::CreateGraph(const std::string& type std::unique_ptr<TOptions> &&opts) const {
    return Impl->CreateGraph(type std::move(opts));
}

TFactory::TFactory() : Impl(std::make_unique<TFactory::TImpl>()) {}
TFactory::~TFactory(){}

std::vector<std::string> TFactory::GetAvailableGraphs() const {
    return Impl->GetAvailableGraphs();
}