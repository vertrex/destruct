#!/usr/bin/python -i
import sys

sys.path.append('../../../')
sys.path.append('../../')
sys.path.append('../')

from _destruct import *
from _workerpool import *

COUNT = 10

workerPool = DStructs().find("WorkerPool").newObject(DUInt8(4))


vector = DStructs().find("DVectorUInt64").newObject()
for x in range(0, COUNT):
  vector.push(x)

taskStruct = DStructs().find("Task")
get = vector.get

#Test task with pure C++ object

for x in range(0, COUNT):
  task = taskStruct.newObject()
  task.function = vector.get 
  task.argument = DUInt64(x)
  workerPool.addTask(task)

print 'workerPool.addTask'
vector = workerPool.join()
for x in vector:
  print x.result

#Test map
print 'Map'
vector = DStructs().find("DVectorUInt64").newObject()

#vector = DStructs().find("TraceObject").newObject(vector)
#print vector


for x in range(0, COUNT):
  vector.push(x)

task = DStructs().find("TaskObject").newObject()
task.function = vector.get
task.argument = vector

for x in workerPool.map(task):
  print x.result
