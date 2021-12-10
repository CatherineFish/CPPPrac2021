#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include "factory.h"

TEST(GraphsCreation, noExceptionTest)
{
	TFactory factory;
	
	//Bipartite Graph
	std::vector<char> bipartParamFirst = {'A','B','C','D'};
	std::vector<char> bipartParamSecond = {'E','F'};
	ASSERT_NO_THROW(factory.Create("bipartite", std::make_unique<TBipartiteOptions>(bipartParamFirst, bipartParamSecond)));
	
	//Complete Graph
	std::vector<char> completeParam = {'A', 'B', 'F'};
	ASSERT_NO_THROW(factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam)));
	
	//SimpleGraph
	std::vector<std::string> simpleParam = {{"EF"}, {"FA"}};
	ASSERT_NO_THROW(factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam)));
	
	//Weighted Graph
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};
	ASSERT_NO_THROW(factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW)));
}

TEST(GraphsCreation, ExceptionTest)
{
	TFactory factory;
	
	//Wrong Vertex Name
	std::vector<char> completeParam = {'7', '8', '9'};
	ASSERT_THROW(factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam)), std::invalid_argument);

	//Wrong Edge Name
	std::vector<std::string> simpleParam_1 = {{"E7"}, {"FA"}, {"AB"}};
	ASSERT_THROW(factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam_1)), std::invalid_argument);

	std::vector<std::string> simpleParam_2 = {{"EM"}, {"F"}, {"AB"}};
	ASSERT_THROW(factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam_2)), std::invalid_argument);

	std::vector<std::string> simpleParam_3 = {{"EM"}, {"FA"}, {"ABC"}};
	ASSERT_THROW(factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam_3)), std::invalid_argument);

	//In Weighted Graph Wrong Edges And Weights Params Number
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}, {"LM"}};
	std::vector<int> weightedParamW = {5, 6};
	ASSERT_THROW(factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW)), std::invalid_argument);

	//In Bipartite Graph intersection of vertices of two parts
	std::vector<char> bipartParamFirst = {'A','B','C','D'};
	std::vector<char> bipartParamSecond = {'E','F','A'};
	ASSERT_THROW(factory.Create("bipartite", std::make_unique<TBipartiteOptions>(bipartParamFirst, bipartParamSecond)), std::invalid_argument);
}

TEST(GraphsMethods, toString)
{
	TFactory factory;
	
	//Bipartite Graph
	std::vector<char> bipartParamFirst = {'A','B','C','D'};
	std::vector<char> bipartParamSecond = {'E','F'};
	auto bipartite = factory.Create("bipartite", std::make_unique<TBipartiteOptions>(bipartParamFirst, bipartParamSecond));
	std::ostringstream expectedB;
    expectedB << "BipartiteGraph {{A, B, C, D}, {E, F}}";
    std::string actualB = bipartite->ToString();
    std::string Bexpected = expectedB.str();
    EXPECT_EQ(Bexpected, actualB);
	
	//Complete Graph
	std::vector<char> completeParam = {'A', 'B', 'F'};
	auto complete = factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam));
	std::ostringstream expectedC;
    expectedC << "CompleteGraph {A, B, F}";
    std::string actualC = complete->ToString();
    std::string Cexpected = expectedC.str();
    EXPECT_EQ(Cexpected, actualC);

	//SimpleGraph
	std::vector<std::string> simpleParam = {{"EF"}, {"FA"}};
	auto simple = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam));
	std::ostringstream expectedS;
    expectedS << "SimpleGraph {AF, EF}";
    std::string actualS = simple->ToString();
    std::string Sexpected = expectedS.str();
    EXPECT_EQ(Sexpected, actualS);

	
	//Weighted Graph
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};
	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));
	std::ostringstream expectedW;
    expectedW << "WeightedGraph {DE:6, DF:5}";
    std::string actualW = weighted->ToString();
    std::string Wexpected = expectedW.str();
    EXPECT_EQ(Wexpected, actualW);
}

TEST(GraphsMethods, GetVertices)
{
	TFactory factory;
	
	//Bipartite Graph
	std::vector<char> bipartParamFirst = {'A','B','C','D'};
	std::vector<char> bipartParamSecond = {'E','F'};
	auto bipartite = factory.Create("bipartite", std::make_unique<TBipartiteOptions>(bipartParamFirst, bipartParamSecond));
	auto Bvertex = bipartite->GetVertices();
	std::vector<char> vertexB = {'A','B','C','D', 'E','F'} ;
	EXPECT_EQ(Bvertex, vertexB);
	
	//Complete Graph
	std::vector<char> completeParam = {'A', 'B', 'F'};
	auto complete = factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam));
	auto Cvertex = complete->GetVertices();
	std::vector<char> vertexC = {'A','B','F'}; 
	EXPECT_EQ(Cvertex, vertexC);

	//SimpleGraph
	std::vector<std::string> simpleParam = {{"EF"}, {"FA"}};
	auto simple = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam));
	auto Svertex = simple->GetVertices();
	std::vector<char> vertexS = {'A','E','F'}; 
	EXPECT_EQ(Svertex, vertexS);

	
	//Weighted Graph
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};
	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));
	auto Wvertex = weighted->GetVertices();
	std::vector<char> vertexW = {'D','E','F'}; 
	EXPECT_EQ(Wvertex, vertexW);

}

TEST(GraphsMethods, GetEdges)
{
	TFactory factory;
	
	//Bipartite Graph
	std::vector<char> bipartParamFirst = {'A','B','C','D'};
	std::vector<char> bipartParamSecond = {'E','F'};
	auto bipartite = factory.Create("bipartite", std::make_unique<TBipartiteOptions>(bipartParamFirst, bipartParamSecond));
	auto Bvertex = bipartite->GetEdges();
	std::vector<std::pair<char,char>> vertexB = {{'A','E'}, {'A','F'}, {'B','E'}, {'B','F'}, {'C','E'}, {'C','F'}, {'D','E'}, {'D','F'}} ;
	EXPECT_EQ(Bvertex, vertexB);
	
	//Complete Graph
	std::vector<char> completeParam = {'A', 'B', 'F'};
	auto complete = factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam));
	auto Cvertex = complete->GetEdges();
	std::vector<std::pair<char,char>> vertexC = {{'A', 'B'}, {'A', 'F'}, {'B', 'F'}}; 
	EXPECT_EQ(Cvertex, vertexC);

	//SimpleGraph
	std::vector<std::string> simpleParam = {{"EF"}, {"FA"}};
	auto simple = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam));
	auto Svertex = simple->GetEdges();
	std::vector<std::pair<char,char>> vertexS = {{'A', 'F'}, {'E','F'}}; 
	EXPECT_EQ(Svertex, vertexS);

	
	//Weighted Graph
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};
	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));
	auto Wvertex = weighted->GetEdges();
	std::vector<std::pair<char,char>> vertexW = {{'D','E'},{'D', 'F'}}; 
	EXPECT_EQ(Wvertex, vertexW);
	
}

TEST(GraphsMethods, AsWeighted)
{
	TFactory factory;
	
	//Bipartite Graph
	std::vector<char> bipartParamFirst = {'A','B','C','D'};
	std::vector<char> bipartParamSecond = {'E','F'};
	auto bipartite = factory.Create("bipartite", std::make_unique<TBipartiteOptions>(bipartParamFirst, bipartParamSecond));
	auto bipartite_2 = bipartite->AsWeighted(5);
	std::ostringstream expectedB;
    expectedB << "WeightedGraph {AE:5, AF:5, BE:5, BF:5, CE:5, CF:5, DE:5, DF:5}";
    std::string actualB = bipartite_2->ToString();
    std::string Bexpected = expectedB.str();
    EXPECT_EQ(Bexpected, actualB);
	
	//Complete Graph
	std::vector<char> completeParam = {'A', 'B', 'F'};
	auto complete = factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam));
	std::ostringstream expectedC;
    auto complete_2 = complete->AsWeighted(5);
    expectedC << "WeightedGraph {AB:5, AF:5, BF:5}";
    std::string actualC = complete_2->ToString();
    std::string Cexpected = expectedC.str();
    EXPECT_EQ(Cexpected, actualC);

	//SimpleGraph
	std::vector<std::string> simpleParam = {{"EF"}, {"FA"}};
	auto simple = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam));
	std::ostringstream expectedS;
    auto simple_2 = simple->AsWeighted(5);
    expectedS << "WeightedGraph {AF:5, EF:5}";
    std::string actualS = simple_2->ToString();
    std::string Sexpected = expectedS.str();
    EXPECT_EQ(Sexpected, actualS);

	
	//Weighted Graph
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};
	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));
	auto weighted_2 = weighted->AsWeighted(3);
	std::ostringstream expectedW;
    expectedW << "WeightedGraph {DE:6, DF:5}";
    std::string actualW = weighted_2->ToString();
    std::string Wexpected = expectedW.str();
    EXPECT_EQ(Wexpected, actualW);
}

TEST(GraphsOperators, SumWithBipartite)
{
	TFactory factory;
	
	//Bipartite Graph
	std::vector<char> bipartParamFirst = {'A','B','C','D'};
	std::vector<char> bipartParamSecond = {'E','F'};
	auto bipartite = factory.Create("bipartite", std::make_unique<TBipartiteOptions>(bipartParamFirst, bipartParamSecond));
	
	std::vector<char> bipartParamFirst_2 = {'M', 'N'};
	std::vector<char> bipartParamSecond_2 = {'K', 'L'};
	auto bipartite_2 = factory.Create("bipartite", std::make_unique<TBipartiteOptions>(bipartParamFirst_2, bipartParamSecond_2));
	
	auto bipartite_3 = *(dynamic_cast<TBipartiteGraph*>(bipartite.get())) + *(dynamic_cast<TBipartiteGraph*>(bipartite_2.get()));
	
	std::ostringstream expectedB;
    expectedB << "BipartiteGraph {{A, B, C, D, M, N}, {E, F, K, L}}";
    std::string actualB = bipartite_3->ToString();
    std::string Bexpected = expectedB.str();
    EXPECT_EQ(Bexpected, actualB);

	
	//Complete Graph
	std::vector<char> completeParam = {'K', 'M'};
	auto complete = factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam));
	
	auto complete_2 = *(dynamic_cast<TBipartiteGraph*>(bipartite.get())) + *(dynamic_cast<TCompleteGraph*>(complete.get()));
	std::ostringstream expectedC;
    expectedC << "SimpleGraph {AE, AF, BE, BF, CE, CF, DE, DF, KM}";
    std::string actualC = complete_2->ToString();
    std::string Cexpected = expectedC.str();
    EXPECT_EQ(Cexpected, actualC);	

	//SimpleGraph
	std::vector<std::string> simpleParam = {{"EF"}};
	auto simple = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam));
	
	auto simple_2 = *(dynamic_cast<TBipartiteGraph*>(bipartite.get())) + *(dynamic_cast<TSimpleGraph*>(simple.get()));
	std::ostringstream expectedS;
    expectedS << "SimpleGraph {AE, AF, BE, BF, CE, CF, DE, DF, EF}";
    std::string actualS = simple_2->ToString();
    std::string Sexpected = expectedS.str();
    EXPECT_EQ(Sexpected, actualS);
	
	//Weighted Graph
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};
	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));
	ASSERT_THROW(*(dynamic_cast<TBipartiteGraph*>(bipartite.get())) + *(dynamic_cast<TWeightedGraph*>(weighted.get())), std::logic_error);

}

TEST(GraphsOperators, SumWithComplete)
{
	TFactory factory;
	
	//Complete Graph
	std::vector<char> completeParam = {'K', 'M'};
	auto complete = factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam));
	
	std::vector<char> completeParam_2 = {'A', 'B', 'C'};
	auto complete_2 = factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam_2));
	

	auto complete_3 = *(dynamic_cast<TCompleteGraph*>(complete_2.get())) + *(dynamic_cast<TCompleteGraph*>(complete.get()));
	std::ostringstream expectedC;
    expectedC << "CompleteGraph {A, B, C, K, M}";
    std::string actualC = complete_3->ToString();
    std::string Cexpected = expectedC.str();
    EXPECT_EQ(Cexpected, actualC);	
	
	//SimpleGraph
	std::vector<std::string> simpleParam = {{"EF"}};
	auto simple = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam));
	
	auto simple_2 = *(dynamic_cast<TCompleteGraph*>(complete.get())) + *(dynamic_cast<TSimpleGraph*>(simple.get()));
	std::ostringstream expectedS;
    expectedS << "SimpleGraph {EF, KM}";
    std::string actualS = simple_2->ToString();
    std::string Sexpected = expectedS.str();
    EXPECT_EQ(Sexpected, actualS);
	
	//Weighted Graph
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};
	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));
	ASSERT_THROW(*(dynamic_cast<TCompleteGraph*>(complete.get())) + *(dynamic_cast<TWeightedGraph*>(weighted.get())), std::logic_error);

}

TEST(GraphsOperators, SumWithSimple)
{
	TFactory factory;
		
	//SimpleGraph
	std::vector<std::string> simpleParam = {{"EF"}};
	auto simple = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam));
	
	std::vector<std::string> simpleParam_2 = {{"AB"}, {"CM"}};
	auto simple_2 = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam_2));
	
	auto simple_3 = *(dynamic_cast<TSimpleGraph*>(simple_2.get())) + *(dynamic_cast<TSimpleGraph*>(simple.get()));
	std::ostringstream expectedS;
    expectedS << "SimpleGraph {AB, CM, EF}";
    std::string actualS = simple_3->ToString();
    std::string Sexpected = expectedS.str();
    EXPECT_EQ(Sexpected, actualS);	
	
	//Weighted Graph
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};
	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));
	ASSERT_THROW(*(dynamic_cast<TSimpleGraph*>(simple.get())) + *(dynamic_cast<TWeightedGraph*>(weighted.get())), std::logic_error);

}

TEST(GraphsOperators, SumWithWeight)
{
	TFactory factory;

	//Weighted Graph
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};
	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));

	std::vector<std::string> weightedParamEdges_2 = {{"FD"}, {"MK"}};
	std::vector<int> weightedParamW_2 = {3, 6};
	auto weighted_2 = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges_2, weightedParamW_2));
	
	auto weighted_3 = *(dynamic_cast<TWeightedGraph*>(weighted_2.get())) + *(dynamic_cast<TWeightedGraph*>(weighted.get()));
	std::ostringstream expectedW;
    expectedW << "WeightedGraph {DE:6, DF:3, KM:6}";
    std::string actualW = weighted_3->ToString();
    std::string Wexpected = expectedW.str();
    EXPECT_EQ(Wexpected, actualW);	
}

TEST(GraphsOperators, SubWithBipartite)
{
	TFactory factory;
	
	//Bipartite Graph
	std::vector<char> bipartParamFirst = {'A','B','C','D'};
	std::vector<char> bipartParamSecond = {'E','F'};
	auto bipartite = factory.Create("bipartite", std::make_unique<TBipartiteOptions>(bipartParamFirst, bipartParamSecond));
	
	std::vector<char> bipartParamFirst_2 = {'M', 'N'};
	std::vector<char> bipartParamSecond_2 = {'K', 'L'};
	auto bipartite_2 = factory.Create("bipartite", std::make_unique<TBipartiteOptions>(bipartParamFirst_2, bipartParamSecond_2));
	
	auto bipartite_3 = *(dynamic_cast<TBipartiteGraph*>(bipartite.get())) - *(dynamic_cast<TBipartiteGraph*>(bipartite_2.get()));
	
	std::ostringstream expectedB;
    expectedB << "BipartiteGraph {{A, B, C, D}, {E, F}}";
    std::string actualB = bipartite_3->ToString();
    std::string Bexpected = expectedB.str();
    EXPECT_EQ(Bexpected, actualB);

	
	//Complete Graph
	std::vector<char> completeParam = {'K', 'M'};
	auto complete = factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam));
	
	auto complete_2 = *(dynamic_cast<TBipartiteGraph*>(bipartite.get())) - *(dynamic_cast<TCompleteGraph*>(complete.get()));
	std::ostringstream expectedC;
    expectedC << "SimpleGraph {AE, AF, BE, BF, CE, CF, DE, DF}";
    std::string actualC = complete_2->ToString();
    std::string Cexpected = expectedC.str();
    EXPECT_EQ(Cexpected, actualC);	

	//SimpleGraph
	std::vector<std::string> simpleParam = {{"EF"}};
	auto simple = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam));
	
	auto simple_2 = *(dynamic_cast<TBipartiteGraph*>(bipartite.get())) - *(dynamic_cast<TSimpleGraph*>(simple.get()));
	std::ostringstream expectedS;
    expectedS << "SimpleGraph {AE, AF, BE, BF, CE, CF, DE, DF}";
    std::string actualS = simple_2->ToString();
    std::string Sexpected = expectedS.str();
    EXPECT_EQ(Sexpected, actualS);
	
	//Weighted Graph
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};
	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));
	auto weighted_2 = *(dynamic_cast<TBipartiteGraph*>(bipartite.get())) - *(dynamic_cast<TWeightedGraph*>(weighted.get()));
	std::ostringstream expectedW;
    expectedW << "SimpleGraph {AE, AF, BE, BF, CE, CF}";
    std::string actualW = weighted_2->ToString();
    std::string Wexpected = expectedW.str();
    EXPECT_EQ(Wexpected, actualW);
	

}

TEST(GraphsOperators, SubWithComplete)
{
	TFactory factory;
	
	//Complete Graph
	std::vector<char> completeParam = {'E', 'F', 'A', 'D'};
	auto complete = factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam));
	
	std::vector<char> completeParam_2 = {'A', 'B', 'C'};
	auto complete_2 = factory.Create("complete", std::make_unique<TCompleteOptions>(completeParam_2));
	

	auto complete_3 = *(dynamic_cast<TCompleteGraph*>(complete_2.get())) - *(dynamic_cast<TCompleteGraph*>(complete.get()));
	std::ostringstream expectedC;
    expectedC << "CompleteGraph {B, C}";
    std::string actualC = complete_3->ToString();
    std::string Cexpected = expectedC.str();
    EXPECT_EQ(Cexpected, actualC);	
	
	//SimpleGraph
	std::vector<std::string> simpleParam = {{"EF"}};
	auto simple = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam));
	
	auto simple_2 = *(dynamic_cast<TCompleteGraph*>(complete.get())) - *(dynamic_cast<TSimpleGraph*>(simple.get()));
	std::ostringstream expectedS;
    expectedS << "SimpleGraph {AD, AE, AF, DE, DF, EF}";
    std::string actualS = simple_2->ToString();
    std::string Sexpected = expectedS.str();
    EXPECT_EQ(Sexpected, actualS);
	
	//Weighted Graph
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};
	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));
	
	auto weighted_2 = *(dynamic_cast<TCompleteGraph*>(complete.get())) - *(dynamic_cast<TWeightedGraph*>(weighted.get()));
	std::ostringstream expectedW;
    expectedW << "SimpleGraph {AD, AE, AF, EF}";
    std::string actualW = weighted_2->ToString();
    std::string Wexpected = expectedW.str();
    EXPECT_EQ(Wexpected, actualW);
}


TEST(GraphsOperators, SubWithSimple)
{
	TFactory factory;
		
	//SimpleGraph
	std::vector<std::string> simpleParam = {{"EF"}, {"MC"}, {"DE"}, {"AE"}};
	auto simple = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam));
	
	std::vector<std::string> simpleParam_2 = {{"AB"}, {"CM"}};
	auto simple_2 = factory.Create("simple", std::make_unique<TSimpleOptions>(simpleParam_2));
	
	auto simple_3 = *(dynamic_cast<TSimpleGraph*>(simple_2.get())) - *(dynamic_cast<TSimpleGraph*>(simple.get()));
	std::ostringstream expectedS;
    expectedS << "SimpleGraph {AB, CM}";
    std::string actualS = simple_3->ToString();
    std::string Sexpected = expectedS.str();
    EXPECT_EQ(Sexpected, actualS);	
	
	//Weighted Graph
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};
	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));
	auto weighted_2 = *(dynamic_cast<TSimpleGraph*>(simple.get())) - *(dynamic_cast<TWeightedGraph*>(weighted.get()));
	std::ostringstream expectedW;
    expectedW << "SimpleGraph {AE, CM, EF}";
    std::string actualW = weighted_2->ToString();
    std::string Wexpected = expectedW.str();
    EXPECT_EQ(Wexpected, actualW);
}

TEST(GraphsOperators, SubWithWeight)
{
	TFactory factory;

	//Weighted Graph
	std::vector<std::string> weightedParamEdges = {{"FD"}, {"ED"}};
	std::vector<int> weightedParamW = {5, 6};
	auto weighted = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges, weightedParamW));

	std::vector<std::string> weightedParamEdges_2 = {{"FD"}, {"MK"}};
	std::vector<int> weightedParamW_2 = {3, 6};
	auto weighted_2 = factory.Create("weighted", std::make_unique<TWeightedOptions>(weightedParamEdges_2, weightedParamW_2));
	
	auto weighted_3 = *(dynamic_cast<TWeightedGraph*>(weighted_2.get())) - *(dynamic_cast<TWeightedGraph*>(weighted.get()));
	std::ostringstream expectedW;
    expectedW << "WeightedGraph {KM:6}";
    std::string actualW = weighted_3->ToString();
    std::string Wexpected = expectedW.str();
    EXPECT_EQ(Wexpected, actualW);	
}


int main(int argc, char * argv[]) {
	testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}