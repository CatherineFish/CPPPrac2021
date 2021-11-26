#pragma once

class oneJob
{
private:
    int duration, tStart = 0;
    size_t num;
public:
    std::vector<oneJob *> dependences;
    std::vector<size_t> depNum;
    int proc;
    
    ~oneJob(){
        std::vector<size_t>().swap(depNum);
        std::vector<oneJob*>().swap(dependences);
    }

    oneJob(int dur, size_t num_): duration(dur), num(num_) 
    {}

    oneJob(int dur, size_t num_, std::vector<size_t> depNum_): duration(dur), num(num_) 
    {
        for (auto i: depNum_){
            depNum.push_back(i);
        }
    }

    void initializeJob(std::vector<oneJob *> dep)
    {
        for (size_t i = 0; i < dep.size(); i++)
        {
            dependences.push_back(dep[i]);
        }
        return;
    }

    int getDuration ()
    {
        return duration;
    }

    int getTStart ()
    {
        return tStart;
    }
    
    size_t getNum ()
    {
        return num;
    }
    
    int getLastTStart()
    {
        int time = 0;
        for (auto i: dependences)
        {
            time = std::max(time, i->getTStart() + i->getDuration());
        }
        return time;
    }

    void setTStart(int time)
    {
        tStart = time;
        return;
    }

};
