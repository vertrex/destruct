#!/usr/bin/python -i
import sys

sys.path.append('../../../')
sys.path.append('../../')
sys.path.append('../')

from _destruct import *
from _workerpool import *

COUNT = 1000000

print "create pool of 4 threads"
workerPool = DStructs().find("WorkerPool").newObject(DUInt8(4))

print "create a vector of ", COUNT, " value"
vector = DStructs().find("DVectorUInt64").newObject()
for x in range(0, COUNT):
  vector.push(x)

taskStruct = DStructs().find("Task")
get = vector.get

#Test task with pure C++ object

print "create task"
for x in range(0, COUNT):
  task = taskStruct.newObject()
  task.function = vector.get 
  task.argument = DUInt64(x)
  workerPool.addTask(task)

print 'waiting results'
vector = workerPool.join()
print "got results (" + str(len(vector)) + ")"
for x in vector:
  #print x.result
  y = x.result

print 'Map'
vector = DStructs().find("DVectorUInt64").newObject()

#vector = DStructs().find("TraceObject").newObject(vector)

print "create a vector of ", COUNT, " value"
for x in range(0, COUNT):
  vector.push(x)

print "create task"
task = DStructs().find("TaskObject").newObject()
task.function = vector.get
task.argument = vector

print "map task"
result = workerPool.map(task)
print "got results (" + str(len(result)) + ")"
for x in result:
#for x in workerPool.map(task):
  #print x.result
  z = x.result 
