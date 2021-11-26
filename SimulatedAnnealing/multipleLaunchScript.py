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
        
