import matplotlib.pyplot as plt

#ParallelTimeFromThreadsNum
xaxis = list(range(1, 10))
yaxis = []

with open("data/PARALLEL_ThreadTime.txt", 'r') as resFile:  
    for line in resFile:
        yaxis += line.split()

print(yaxis)
print(xaxis)
xaxis = [x for _, x in sorted(zip(yaxis, xaxis))]  
yaxis.sort()
print(yaxis)
print(xaxis)
fig, ax = plt.subplots(figsize = (10, 10))
ax.plot(xaxis, yaxis)
ax.set(xlabel="threads num", ylabel="time, seconds")
ax.set_title("Параллельный алгоритм. Зависимость времени работы от числа потоков")  
fig.savefig("data/graph/ParallelTimeFromThreadsNum.png")  


#ParallelTimeFromJobNum
xaxis = list(range(10, 1000, 100))
yaxis = []
  
with open("data/PARALLEL_JobTime.txt", 'r') as resFile:  
    for line in resFile:
        yaxis += line.split()
xaxis = [x for _, x in sorted(zip(yaxis, xaxis))]  
yaxis.sort()
fig, ax = plt.subplots(figsize = (10, 10))
ax.plot(xaxis, yaxis)
ax.set(xlabel="jobs num", ylabel="time, seconds")
ax.set_title("Параллельный алгоритм. Зависимость времени работы от числа работ")  
fig.savefig("data/graph/ParallelTimeFromJobsNum.png")  

#Parallel acceleration threads
xaxis = list(range(1, 10))
yaxis = []
fig, ax = plt.subplots(figsize = (10, 10))
with open("data/ACC_ThreadTime.txt", 'r') as resFile:  
    for line in resFile:
        yaxis += line.split()
xaxis = [x for _, x in sorted(zip(yaxis, xaxis))]  
yaxis.sort()
ax.plot(xaxis, yaxis)
ax.set(xlabel="threads num", ylabel="times")
ax.set_title("Параллельный алгоритм. Зависимость ускорения от числа потоков")  
fig.savefig("data/graph/ParallelAccelerationFromThreadsNum.png")  


#Parallel acceleration jobs
xaxis = list(range(10, 1000, 100))
yaxis = []
fig, ax = plt.subplots(figsize = (10, 10))  
with open("data/ACC_JobTime.txt", 'r') as resFile:  
    for line in resFile:
        yaxis += line.split()
xaxis = [x for _, x in sorted(zip(yaxis, xaxis))]  
yaxis.sort()
ax.plot(xaxis, yaxis)
ax.set(xlabel="jobs num", ylabel="times")
ax.set_title("Параллельный алгоритм. Зависимость ускорения от числа работ")  
fig.savefig("data/graph/ParallelAccelerationFromJobsNum.png")  


#TimeFromJobNum
xaxis = list(range(10, 1000, 100))
yaxis = []
fig, ax = plt.subplots(figsize = (10, 10)) 
with open("data/JobTime.txt", 'r') as resFile:  
    for line in resFile:
        yaxis += line.split()
xaxis = [x for _, x in sorted(zip(yaxis, xaxis))]  
yaxis.sort()
ax.plot(xaxis, yaxis)
ax.set(xlabel="jobs num", ylabel="time, seconds")
ax.set_title("Последовательный алгоритм. Зависимость времени работы от числа работ")  
fig.savefig("data/graph/TimeFromJobsNum.png")  


#TimeFromProcNum
xaxis = list(range(10, 200, 20))
yaxis = []
  
with open("data/ProcTime.txt", 'r') as resFile:  
    for line in resFile:
        yaxis += line.split()
xaxis = [x for _, x in sorted(zip(yaxis, xaxis))]  
yaxis.sort()
fig, ax = plt.subplots(figsize = (10, 10))
ax.plot(xaxis, yaxis)
ax.set(xlabel="processors num", ylabel="time, seconds")
ax.set_title("Последовательный алгоритм. Зависимость времени работы от числа процессоров")  
fig.savefig("data/graph/TimeFromProcessorsNum.png")  
