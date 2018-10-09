#!/usr/bin/env python3
import matplotlib.pyplot as plt
import numpy as np
import math


#EXTRACT RESULTS
#SEQUENTIAL
linesSeq = open('./results/gen_seq2.txt', 'r')
charsSeq = [line.rstrip('\n') for line in linesSeq]

temp=[]
seq=[]
allComps=[]
for line in charsSeq:
	tokens= line.split(', ',2)
	allComps.append(float(tokens[1])) #comp Time

print("\nallComps: ")
print(allComps)

allComps.remove(max(allComps))
allComps.remove(min(allComps))
seqTime=sum(allComps)/len(allComps)

print("\nseq: ")
print(seqTime)

#THREADS
lines = open('./results/gen_threads2.txt', 'r')
chars = [line.rstrip('\n') for line in lines]

workers=[]
Th=[]
for line in chars:
	tokens= line.split(', ', 4)
	if int(tokens[0]) not in workers:
		workers.append(int(tokens[0]))
	Th.append( (float(tokens[0]), float(tokens[2])) ) #comp

		
print("\nworkers: ")
print(workers)
print("\Th: ")
print(Th)
	
temp=[]
yTh=[]
w=1.0
for x,y in Th:
	if (w==x):	
		temp.append(y);
	else:
		temp.remove(max(temp))
		temp.remove(min(temp))	
		yTh.append(sum(temp)/len(temp));	
		del temp[:]
		tmp=[y]
		w*=2
temp.remove(max(temp))
temp.remove(min(temp))	
yTh.append(sum(temp)/len(temp));	

tmp=[]
print("\nxTh: ")
print(workers)
print("\nyTh: ")
print(yTh)

scalability_1=[]
speedup_1=[]
efficiency_1=[]
ideal_1=[]
i=0
for item in yTh:
	scalability_1.append(yTh[0]/item)
	speedup_1.append(seqTime/item)
	efficiency_1.append(speedup_1[i]/workers[i])
	ideal_1.append(seqTime/workers[i])
	i+=1

print("\ny LESS TREES TH: ")
print("\nideal comp time: ")
print( ideal_1)
print("\nscalability: ")
print( scalability_1)
print("\nspeedup: ")
print( speedup_1 )
print("\nefficiency: " )
print(efficiency_1)

#FASTFLOW
lines = open('./results/gen_ff2.txt', 'r')
chars = [line.rstrip('\n') for line in lines]

            
ff=[]
for line in chars:
	tokens= line.split(', ',3)
	ff.append( (float(tokens[0]), float(tokens[2])) ) #comp Time	

print("\nff: ")
print(ff)
	
temp=[]
yFf=[]
w=1.0
for x,y in ff:
	if (w==x):	
		temp.append(y);
	else:
		temp.remove(max(temp))
		temp.remove(min(temp))	
		yFf.append(sum(temp)/len(temp));	
		temp=[y]
		w*=2
temp.remove(max(temp))
temp.remove(min(temp))	
yFf.append(sum(temp)/len(temp));	

tmp=[]
print("\nxFf: ")
print(workers)
print("\nyFf: ")
print(yFf)	
	
scalability_ff=[]
speedup_ff=[]
efficiency_ff=[]

i=0
for item in yFf:
	scalability_ff.append(yFf[0]/item)
	speedup_ff.append(seqTime/item)
	efficiency_ff.append(speedup_ff[i]/workers[i])
	i+=1

print("\ny LESS TREES FF: ")
print("\nscalability: ")
print( scalability_ff)
print("\nspeedup: ")
print( speedup_ff )
print("\nefficiency: " )
print(efficiency_ff)

#COMPLETION TIME
#-------std
plt.figure(1)
plt.plot(workers, yTh, marker='x',label='Thread ')
plt.plot(workers, yFf, marker='x',label='FastFlow')
plt.plot(workers, ideal_1 ,linestyle='--',label='Ideal Comp Time')
plt.plot(np.arange(0, max(workers)+1, 1), [seqTime]*(max(workers)+1), label='sequential')
plt.xlabel('PARALLELISM DEGREE')
plt.ylabel('COMPLETION TIME')
plt.ylim((-0.5, 10.0))
plt.xlim((0, max(workers)+1))
plt.yticks(np.arange(-0.5, float(max(workers)+1), 1.0))
plt.xticks([1,2,4,6,8,9])
plt.legend()
plt.grid()
plt.show()

#SCALABILITY
#-------std
plt.figure(2)
plt.plot(workers, scalability_1, marker='x', label='Thread')
plt.plot(workers, scalability_ff, marker='x', label='FastFlow')
plt.plot(workers, workers,linestyle='--', label='Ideal Scalability')
plt.xlabel('PARALLELISM DEGREE')
plt.ylabel('SCALABILITY')
plt.ylim((-0.5, max(workers)))
plt.xlim((0, max(workers)+1))
plt.xticks([1,2,4,6,8,9])
plt.legend()
plt.grid()
plt.show()

#SPEEDUP
#-------std
plt.figure(3)
plt.plot(workers, speedup_1,marker='x', label='Thread')
plt.plot(workers, speedup_ff,marker='x', label='FastFlow')
plt.plot(workers, workers,linestyle='--', label='Ideal Speedup')
plt.xlabel('PARALLELISM DEGREE')
plt.ylabel('SPEEDUP')
plt.ylim((-0.5, max(workers)))
plt.xlim((0, max(workers)+1))
plt.xticks([1,2,4,6,8,9])
plt.legend()
plt.grid()
plt.show()

#EFFICIENCY
#-------std
plt.figure(4)
plt.plot(workers, efficiency_1,marker='x', label='Thread')
plt.plot(workers, efficiency_ff,marker='x', label='FastFlow')
plt.plot( workers, [1]*len(workers), linestyle='--', label='Ideal Efficiency')
plt.xlabel('PARALLELISM DEGREE')
plt.ylabel('EFFICIENCY')
plt.ylim((0.0, 1.5))
plt.xlim((0, max(workers)+1))
plt.xticks([1,2,4,6,8,9])
plt.legend()
plt.grid()
plt.show()

