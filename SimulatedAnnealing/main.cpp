#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <chrono>

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

        generator * newGen = new generator(vertexNum, minWeight, maxWeight, dispVertexWeight,
                                           compNum, dispComp, vertexDegree, dispDegree);    
        newGen->compInitialize();
        newGen->degreeInitialize();

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
        solution * sol = parser->initSolution(allJobs, procNum);
        temperature *t = new temperature(initTemp);
        mutation * mut = new mutation();

        
        std::cout << "0 - Parallel Algorithm, 1 - Sequential Algorithm:" << std::endl;
        std::cin >> mod;
        solution * res;
        if (!mod) {
            int threadNum;
            std::cout << "Print number of threads:" << std::endl;
            std::cin >> threadNum;
            auto start_time_paral = std::chrono::high_resolution_clock::now();
            parallelAlgorithm parallelAlg(threadNum, sol, t, mut);
            res = parallelAlg.parralelSolution();
            auto end_time_paral = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> worktime = end_time_paral - start_time_paral;
            std::cout << worktime.count() / 1000 << std::endl;
            res->printRESULTNumFile(outFilename);
            
            int disbalance = res->getCriterion();
            int duration = res->duration(); 
            int idling = res->idle();
            res->print();
            std::cout << std::endl;    
            res->printNum();
            std::cout << std::endl;    
            std::cout << "Solution characteristics:" << std::endl;
            std::cout << "Duration : " << duration << std::endl;
            std::cout << "Disbalance : " << disbalance << std::endl;
            std::cout << "Idle time : " << idling << std::endl;
        } else {
            std::cout << "Print steps for intermediate solution print or -1:" << std::endl; 
            std::cin >> stepsForPrint;
            if (stepsForPrint != -1) {
                std::cout << "Print intermediate solution print filename:" << std::endl; 
                std::cin >> interFilename;
            }
            auto start_time_alg = std::chrono::high_resolution_clock::now();
            mainAlgorithm alg(sol, t, mut, nullptr, stepsForPrint);
            
            if (stepsForPrint != -1) {
                res = alg.mainCycle(interFilename);       
            } else {
                res = alg.mainCycle();    
            }
            auto end_time_alg = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> worktime = end_time_alg - start_time_alg;
            std::cout << worktime.count() / 1000 << std::endl;
            res->printRESULTNumFile(outFilename);
            
            int disbalance = res->getCriterion();
            int duration = res->duration(); 
            int idling = res->idle();
            res->print();
            std::cout << std::endl;    
            res->printNum();
            std::cout << std::endl;    
            std::cout << "Solution characteristics:" << std::endl;
            std::cout << "Duration : " << duration << std::endl;
            std::cout << "Disbalance : " << disbalance << std::endl;
            std::cout << "Idle time : " << idling << std::endl;
        }
    } else if (mod == 2) {
        std::string fileWithSchedule, fileWithJobs;
        std::cout << "Print Schedule filename :" << std::endl; 
        std::cin >> fileWithSchedule;
        std::cout << "Print input filename :" << std::endl; 
        std::cin >> fileWithJobs;
        scheduleCharact(fileWithSchedule, fileWithJobs);
    }
    /*
    int mod = std::atoi(argv[1]);
    if (!mod) {
        int vertexNum = std::atoi(argv[2]);
        int minWeight = std::atoi(argv[3]), maxWeight = std::atoi(argv[4]);
        double dispVertexWeight = std::atof(argv[5]);
        double compNum = std::atof(argv[6]), dispComp = std::atof(argv[7]);
        double vertexDegree = std::atof(argv[8]), dispDegree = std::atof(argv[9]);  
        std::string txtFileName, dotFileName;  
        
        generator * newGen = new generator(vertexNum, minWeight, maxWeight, dispVertexWeight,
                                           compNum, dispComp, vertexDegree, dispDegree);    
        newGen->compInitialize();
        newGen->degreeInitialize();

        txtFileName = argv[10];
        dotFileName = argv[11];
        
        newGen->inputFormat(txtFileName);
        newGen->dotFormat(dotFileName);
    } else if (mod == 1) {
        std::srand(std::time(nullptr));
        int procNum, stepsForPrint;
        double initTemp;
        std::string inFilename, outFilename, interFilename;

        inFilename = argv[2];
        procNum = std::atoi(argv[3]);
        initTemp = std::atof(argv[4]);
        outFilename = argv[5];
        std::vector<oneJob *> allJobs = parser(inFilename);
        parseJobs * parser = new parseJobs();
        solution * sol = parser->initSolution(allJobs, procNum);
        temperature *t = new temperature(initTemp);
        mutation * mut = new mutation();
        
        mod = std::atoi(argv[6]);
        solution * res;
        if (!mod) {
            int threadNum;
            threadNum = std::atoi(argv[7]);
            auto start_time_paral = std::chrono::high_resolution_clock::now();
            parallelAlgorithm parallelAlg(threadNum, sol, t, mut);
            res = parallelAlg.parralelSolution();
            auto end_time_paral = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> worktime = end_time_paral - start_time_paral;
            std::cout << worktime.count() / 1000 << std::endl;
            res->printRESULTNumFile(outFilename);

        } else {
            stepsForPrint = std::atoi(argv[7]);
            auto start_time_alg = std::chrono::high_resolution_clock::now();
            mainAlgorithm alg(sol, t, mut, nullptr, stepsForPrint);
            if (stepsForPrint != -1) {
                res = alg.mainCycle(interFilename);       
            } else {
                res = alg.mainCycle();    
            }
            auto end_time_alg = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> worktime = end_time_alg - start_time_alg;
            std::cout << worktime.count() / 1000 << std::endl;
            res->printRESULTNumFile(outFilename);
    
        }

        
        //res->print();
        
    } else if (mod == 2) {
        std::string fileWithSchedule, fileWithJobs;
        fileWithSchedule = argv[2];
        fileWithJobs = argv[3];
        scheduleCharact(fileWithSchedule, fileWithJobs);
    }*/
    return 0;
}