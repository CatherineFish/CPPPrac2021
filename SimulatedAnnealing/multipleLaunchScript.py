import subprocess
from functools import reduce

#time from threads GENERATION
subprocess.check_call(["./main 0 20 10 100 20 3 2 3 2 data/PARALLEL_TimeFromThreadNum data/PARALLEL_TimeFromThreadNum"], shell=True)


#PARALLEL: time from threads        
for i in range(20):
    for thread in range(1, 10):
        filename = "data/PARALLEL_threadTest_" + str(thread) + ".txt" #result solution
        result = "data/PARALLEL_THREAD_" + str(thread) + ".txt" #time
        subprocess.check_call([f"./main 1 data/PARALLEL_TimeFromThreadNum.txt 5 500 {filename} 0 {thread} >> {result}"], shell=True)

with open("data/PARALLEL_ThreadTime.txt", 'w') as resFile:  
    resLine = ""
    for thread in range(1, 10):
        curFile = "data/PARALLEL_THREAD_" + str(thread) + ".txt"
        with open(curFile, 'r') as file:
            for line in file:
                sumTime = reduce(lambda a, b: float(a) + float(b), (line.split()))
            resLine += str(sumTime) + " "
    resFile.write(resLine)

print("FIRST TEST\n")
        
#PARALLEL: time from job num
for i in range(20):
    for job in range(10, 1000, 100):
        inFile = "data/PARALLEL_TimeFromJobNum" + str(job)
        subprocess.check_call([f"./main 0 {job} 10 100 20 3 2 3 2 {inFile} {inFile}"], shell=True)
        inFile += ".txt"
        filename = "data/PARALLEL_jobTest_" + str(job) + ".txt"
        result = "data/PARALLEL_JOB_" + str(job) + ".txt"
        subprocess.check_call([f"./main 1 {inFile} 5 500 {filename} 0 4 >> {result}"], shell=True)

with open("data/PARALLEL_JobTime.txt", 'w') as resFile:  
    resLine = ""
    for job in range(10, 1000, 100):
        curFile = "data/PARALLEL_JOB_" + str(job) + ".txt"
        with open(curFile, 'r') as file:
            for line in file:
                sumTime = reduce(lambda a, b: float(a) + float(b), (line.split()))
            resLine += str(sumTime) + " "
    resFile.write(resLine)

print("SECOND TEST\n")

#PARALLEL: acceleration
with open("data/ACC_ThreadTime.txt", 'w') as resFile:
    resLine = ""  
    with open ("data/PARALLEL_ThreadTime.txt", "r") as inFile:
        for line in inFile:
            line = line.split()
            initTime = float(line[0]); 
            for elem in line:
                resLine += str(float(elem) / initTime) + " "
    resFile.write(resLine)

with open("data/ACC_JobTime.txt", 'w') as resFile:  
    resLine = ""  
    with open ("data/PARALLEL_JobTime.txt", "r") as inFile:
        for line in inFile:
            line = line.split()
            initTime = float(line[0]); 
            for elem in line:
                resLine += str(float(elem) / initTime) + " "
    resFile.write(resLine)

print("THIRD TEST\n")

#time from job num
for i in range(20):
    for job in range(10, 1000, 100):
        inFile = "data/TimeFromJobNum" + str(job)
        subprocess.check_call([f"./main 0 {job} 10 100 20 3 2 3 2 {inFile} {inFile}"], shell=True)
        inFile += ".txt"
        filename = "data/jobTest_" + str(job) + ".txt"
        result = "data/JOB_" + str(job) + ".txt"
        subprocess.check_call([f"./main 1 {inFile} 5 500 {filename} 1 -1 >> {result}"], shell=True)

with open("data/JobTime.txt", 'w') as resFile:  
    resLine = ""
    for job in range(10, 1000, 100):
        curFile = "data/JOB_" + str(job) + ".txt"
        with open(curFile, 'r') as file:
            for line in file:
                sumTime = reduce(lambda a, b: float(a) + float(b), (line.split()))
            resLine += str(sumTime) + " "
    resFile.write(resLine)

print("FOUTH TEST\n")

#time from proc num
inFile = "data/TimeFromProcNum"
subprocess.check_call([f"./main 0 200 10 100 20 3 2 3 2 {inFile} {inFile}"], shell=True)
inFile += ".txt"
        
for i in range(20):
    for proc in range(10, 200, 20):
        filename = "data/procTest_" + str(proc) + ".txt"
        result = "data/PROC_" + str(proc) + ".txt"
        subprocess.check_call([f"./main 0 {inFile} {proc} 500 {filename} 1 -1 >> {result}"], shell=True)

with open("data/ProcTime.txt", 'w') as resFile:  
    resLine = ""
    for job in range(10, 1000, 100):
        curFile = "data/PROC_" + str(job) + ".txt"
        with open(curFile, 'r') as file:
            for line in file:
                sumTime = reduce(lambda a, b: float(a) + float(b), (line.split()))
            resLine += str(sumTime) + " "
    resFile.write(resLine)

print("FIFTH TEST\n")

subprocess.check_call(["rm data/PARALLEL_threadTest_* data/PARALLEL_THREAD_*"], shell=True)
subprocess.check_call(["rm data/PARALLEL_jobTest_* data/PARALLEL_JOB_*"], shell=True)
subprocess.check_call(["rm data/jobTest_* data/JOB_*"], shell=True)
subprocess.check_call(["rm data/procTest_* data/PROC_*"], shell=True)


#

