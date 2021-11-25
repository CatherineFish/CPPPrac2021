#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <fstream>

#include "mainAlgorithm.h"
#include "solution.h"
#include "mutation.h"
#include "temperature.h"
#include "parallelAlg.h"
#include "parser.h"
#include "generator.h"


int main(int argc, char * argv[])
{
    int mod;
    std::cout << "0 - Generate data, 1 - Algorithm launch, 2 - Schedule analysis:" << std::endl;
    std::cin >> mod;

    if (!mod) {
        int vertexNum;
        int minWeight, maxWeight;
        double dispVertexWeight;
        double compNum, dispComp;
        double vertexDegree, dispDegree;  
        std::string txtFileName, dotFileName;  
        
        std::cout << "Print number of vertices:" << std::endl;
        std::cin >> vertexNum;
        std::cout << "Print minimum vertex weight:" << std::endl;
        std::cin >> minWeight;
        std::cout << "Print maximum vertex weight:" << std::endl;
        std::cin >> maxWeight;
        std::cout << "Print variance of vertex weights:" << std::endl;
        std::cin >> dispVertexWeight;
        std::cout << "Print average number of connected components:" << std::endl;
        std::cin >> compNum;
        std::cout << "Print variance of connected components:" << std::endl;
        std::cin >> dispComp;
        std::cout << "Print average vertex degree:" << std::endl;
        std::cin >> vertexDegree;
        std::cout << "Print vertex degree variance:" << std::endl;
        std::cin >> dispDegree;

        generator * newGen = nem generator(vertexNum, minWeight, maxWeight, dispVertexWeight,
                                           compNum, dispComp, vertexDegree, dispDegree);    
        newGen->compInitialize();
        newGen->degreeInitialize(vertexDegree, dispDegree);

        std::cout << "Print txt file name (without .txt):" << std::endl; 
        std::cin >> txtFileName;
        std::cout << "Print dot file name (without .dot):" << std::endl;
        std::cin >> dotFileName;
        
        newGen->inputFormat(txtFileName);
        newGen->dotFormat(dotFileName);
    } else if (mod == 1) {
        std::srand(std::time(nullptr));
        int procNum, stepsForPrint;
        double initTemp;
        std::string inFilename, outFilename, interFilename;

        std::cout << "Print input filename:" << std::endl; 
        std::cin >> inFilename;
        
        std::cout << "Print processor number:" << std::endl; 
        std::cin >> procNum;
        
        std::cout << "Print initial temperature:" << std::endl; 
        std::cin >> initTemp;
        
        
        std::cout << "Print outputFile:" << std::endl; 
        std::cin >> outFilename;

        std::vector<oneJob *> allJobs = parser(inFilename);
        parseJobs * parser = new parseJobs();
        solution * sol = parser->initSol(allJobs, procNum);
        temperature *t = new temperature(initTemp);
        mutation * mut = new mutation();

        std::cout << "0 - Parallel Algorithm, 1 - Sequential Algorithm:" << std::endl;
        std::cin >> mod;

        if (!mod) {
            int threadNum;
            std::cout << "Print number of threads:" << std::endl;
            std::cin >> threadNum;
            
            parallelAlgorithm parallelAlg(th_am, sol, t, mut);
            solution * res = parallelAlg.parralelSolution();

        } else {
            std::cout << "Print steps for intermediate solution print or -1:" << std::endl; 
            std::cin >> stepsForPrint;
            if (stepsForPrint != -1) {
                std::cout << "Print intermediate solution print filename:" << std::endl; 
                std::cin >> interFilename;
            }
    
            mainAlgorithm alg(sol, t, mut, nullptr, stepsForPrint);
            
            if (stepsForPrint != -1) {
                solution *res = alg.mainCycle(interFilename);       
            } else {
                solution *res = alg.mainCycle();    
            }    
        }

        
        res->print();
        res->printRESULTNumFile(outFilename);
    } else if (mod == 2) {
        std::string fileWithSchedule, fileWithJobs;
        std::cout << "Print Schedule filename :" << std::endl; 
        std::cin >> fileWithSchedule;
        std::cout << "Print input filename :" << std::endl; 
        std::cin >> fileWithJobs;
        scheduleCharact(fileWithSchedule, fileWithSchedule);
    }
    return 0;
}