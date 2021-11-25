std::vector<oneJob *> parser(std::string filename) {

    std::string line;
    std::vector<oneJob *> allJobs;
    size_t i = 0;
    int N;
    std::ifstream file(filename);
    while (std::getline(file, line)){
        if (line[0] == '#' || line[0] == ' ') continue;
        else {
            std::istringstream curLine(line);
            curLine >> N;    
            break;
        }
    }
    while (std::getline(file, line))
    {
        if (line[0] == '#' || line[0] == ' ') continue;
        std::istringstream jobLine(line);
        int jobDur, dependence;
        jobLine >> jobDur;

        oneJob* newJob = new oneJob(jobDur, i);
        i++;
        
        while (jobLine >> dependence) {
            if (dependence >= N) {
                std::cout << "Invalid input" << std::endl;
                exit(0);
            }
            newJob->depNum.push_back(dependence);
        }
        allJobs.push_back(newJob);
    }
    if (i < N) {
        std::cout << "Invalid input" << std::endl;
        exit(0);
    }
        
    return allJobs;
}

std::vector<std::vector<int>> parseShedule(std::string filename) {
    std::string line;
    std::vector<std::vector<int>> schedule;
    int N;
    size_t i = 0;
    std::ifstream file(filename);
    while (std::getline(file, line)){
        if (line[0] == '#' || line[0] == ' ') continue;
        else {
            std::istringstream curLine(line);
            curLine >> N;    
            break;
        }
    }
    while (std::getline(file, line))
    {
        if (line[0] == '#' || line[0] == ' ') continue;
        //std::cout <<"WOW" << std::endl;
        std::istringstream procLine(line);
        int procNum, newJob;
        char c;
        procLine >> procNum;
        procLine >> c;
        std::vector<int> newVect;
        
        i++;
        
        while (procLine >> newJob) {
            newVect.push_back(newJob);
        }
        schedule.push_back(newVect);
    }
    if (i < N) {
        std::cout << "Invalid input" << std::endl;
        exit(0);
    }
    return schedule;
}

int compare (oneJob* arg1, oneJob * arg2) {
    for (auto dep_1: arg1->depNum) {
        if (dep_1 == arg2->getNum()) {
            return 1;
        }
    }

    for (auto dep_2: arg2->depNum) {
        if (dep_2 == arg1->getNum()) {
            return 0;
        }
    }
    return 0;
}

void scheduleCharact(std::string fileWithSchedule, std::string fileWithJobs) {
    std::vector<oneJob *> allJobs = parser(fileWithJobs);
    std::vector<std::vector<int>> schedule = parseShedule(fileWithSchedule);
    std::vector<std::vector<oneJob*>> jobSchedule;
    for (size_t i = 0; i < schedule.size(); i++) {
        std::vector<oneJob*> newElem;
        for (size_t j = 0; j < schedule[i].size(); j++) {
            newElem.push_back(allJobs[schedule[i][j]]);
            allJobs[schedule[i][j]]->proc = i;
        }
        jobSchedule.push_back(newElem);
    }

    for (size_t i = 0; i < allJobs.size() - 1; i++)
    {
        for (size_t j = 0; j < allJobs.size() - 1 - i; j++)
        {
            if (compare(allJobs[j], allJobs[j + 1])){
                oneJob * job = allJobs[j + 1];
                allJobs.erase(allJobs.begin() + j + 1);
                allJobs.insert(allJobs.begin() + j, job);
            }
        }
    }

    for (size_t i = 0; i < allJobs.size(); i++) {
        int mainStart = 0;
        for (size_t j = 0; j < schedule[allJobs[i]->proc].size(); j++) {
            jobSchedule[allJobs[i]->proc][j]->setTStart(std::max(mainStart, jobSchedule[allJobs[i]->proc][j]->getLastTStart()));
            mainStart = jobSchedule[allJobs[i]->proc][j]->getTStart() + jobSchedule[allJobs[i]->proc][j]->getDuration();
            if (jobSchedule[allJobs[i]->proc][j] == allJobs[i]) {
                break;
            }
        }
    }

    solution * sol = new solution(schedule.size(), jobSchedule);
    int disbalance = sol->getCriterion();
    int duration = sol->duration(); 
    int idling = sol->idle();
    std::cout << "Solution characteristics:" << std::endl;
    std::cout << "Duration : " << duration << std::endl;
    std::cout << "Disbalance : " << disbalance << std::endl;
    std::cout << "Idle time : " << idling << std::endl;
    return;

}


class parseJobs {
private:
    std::vector<std::vector<oneJob *>> jobsGroups;
public:
    parseJobs () {}

    
    
    void bubbleSort(int k)
    {
        for (size_t i = 0; i < jobsGroups[k].size() - 1; i++)
        {
            for (size_t j = 0; j < jobsGroups[k].size() - 1 - i; j++)
            {
                if (compare(jobsGroups[k][j], jobsGroups[k][j + 1])){
                    oneJob * job = jobsGroups[k][j + 1];
                    jobsGroups[k].erase(jobsGroups[k].begin() + j + 1);
                    jobsGroups[k].insert(jobsGroups[k].begin() + j, job);
                }
            }
    
        }    
         
        return;
    }
    
    
    solution * initSol (std::vector<oneJob*> allJobs, int procNum) {
        std::vector<std::vector<oneJob *>> initSol;
        for (size_t i = 0; i < allJobs.size(); i++) {
            bool isInGroup = false;
            for (size_t j = 0; j < jobsGroups.size(); j++) {
                for (size_t k = 0; k < jobsGroups[j].size(); k++) {
                    if (isInGroup) break;
                    for (auto groupDep: jobsGroups[j][k]->depNum) {
                        if (groupDep == allJobs[i]->getNum()) {
                            jobsGroups[j].push_back(allJobs[i]);
                            isInGroup = true;
                        }
                    }
                    if (!isInGroup) {
                        for (auto jobDep: allJobs[i]->depNum) {
                            if (jobDep == jobsGroups[j][k]->getNum()){
                                jobsGroups[j].push_back(allJobs[i]);
                                isInGroup = true;
                            }
                        }
                    }
                }
                
            }
            if (!isInGroup) {
                std::vector<oneJob *> newGroup;
                newGroup.push_back(allJobs[i]);
                jobsGroups.push_back(newGroup);
                
            }
        }
    
        
        for (size_t i = 0; i < jobsGroups.size(); i++) {
            bubbleSort(i);
        }
    
    
        for(auto job: allJobs) {
            std::vector<oneJob*> curDep;
            for (auto k: job->depNum)
            {
                curDep.push_back(allJobs[k]);
            }
            job->initializeJob(curDep);
        }        
        
        for (size_t i = 0; i < procNum; i++) {
            std::vector<oneJob * > newVec;
            initSol.push_back(newVec);
        }
        
        for (size_t j = 0; j < jobsGroups.size(); j++) {
            for (size_t i = 0; i < jobsGroups[j].size(); i++) {
                initSol[j % procNum].push_back(jobsGroups[j][i]);    
            }
                
        }
        
        for (size_t j = 0; j < initSol.size(); j++) {
            int time = 0;
            for (size_t i = 0; i < initSol[j].size(); i++)
            {
                initSol[j][i]->setTStart(time);
                time += initSol[j][i]->getDuration();
            }    
        }    
    
        return new solution(procNum, initSol);
    }

};