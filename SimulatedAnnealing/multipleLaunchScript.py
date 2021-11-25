import subprocess
from functools import reduce

#time from threads GENERATION
subprocess.check_call(["./main 0 20 10 100 20 3 2 3 2 data/TimeFromThreadNum data/TimeFromThreadNum"], shell=True)


#time from threads        
for i in range(20):
    for thread in range(1, 10):
        filename = "data/threadTest_" + thread + ".txt" #result solution
        result = "data/THREAD_" + thread + ".txt" #time
        subprocess.check_call([f"./main 1 data/TimeFromThreadNum.txt 5 500 {filename} 0 {thread} >> {result}"], shell=True)

with open("data/ThreadTime.txt", 'w') as resFile:  
    for thread in range(1, 10):
        curFile = "data/THREAD_" + thread + ".txt"
        with open(curFile, 'r') as file:
            for line in file:
                sumTime = reduce(lambda a, b: int(a) + int(b), (line.split(" ")))
    resFile.write(sumTime)
    resFile.write(" ")

#time from job num
for i in range(20):
    for job in range(10, 1000, 100):
        inFile = "data/TimeFromJobNum" + job
        subprocess.check_call([f"./main 0 {job} 10 100 20 3 2 3 2 {inFile} {inFile}"], shell=True)
        inFile += ".txt"
        filename = "data/jobTest_" + job + ".txt"
        result = "data/JOB_" + job + ".txt"
        subprocess.check_call([f"./main 1 {inFile} 5 500 {filename} 0 {job} >> {result}"], shell=True)

with open("data/JobTime.txt", 'w') as resFile:  
    for job in range(10, 1000, 100):
        curFile = "data/JOB_" + job + ".txt"
        with open(curFile, 'r') as file:
            for line in file:
                sumTime = reduce(lambda a, b: int(a) + int(b), (line.split(" ")))
    resFile.write(sumTime)
    resFile.write(" ")

    

for works in range(10, 1000, 100):
        subprocess.check_call(["./main 1 0 >> Result.txt"], shell=True)
        subprocess.check_call(["../cmake-build-debug/Simulated_annealing 0 1 >> Result.txt"], shell=True)
        subprocess.check_call(["../cmake-build-debug/Simulated_annealing 0 2 >> Result.txt"], shell=True)