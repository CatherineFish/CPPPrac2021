#include <memory>
#include <vector>
#include "graphLib.h"

class TFactory {
	class TImpl;
	std::unique_ptr<const TImpl> Impl;
public:

	TFactory();
	~TFactory();
	std::unique_ptr<TGraph> CreateGraph(
		const std::string& type,
		std::unique_ptr<TOptions>&& opts) const;
	std::vector<std::string> GetAvailableGraphs() const;
};