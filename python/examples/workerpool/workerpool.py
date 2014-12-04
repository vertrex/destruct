#!/usr/bin/python -i
import sys

sys.path.append('../../../')
sys.path.append('../../')
sys.path.append('../')

from _destruct import *
from _workerpool import *

COUNT = 10

workerPool = Destruct().find("WorkerPool").newObject(DUInt8(4))


vector = Destruct().find("DVectorUInt64").newObject()
for x in range(0, COUNT):
  vector.push(x)

taskStruct = Destruct().find("Task")
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
vector = Destruct().find("DVectorUInt64").newObject()

#vector = Destruct().find("DebugObject").newObject(vector)
#print vector


for x in range(0, COUNT):
  vector.push(x)

task = Destruct().find("TaskObject").newObject()
task.function = vector.get
task.argument = vector

for x in workerPool.map(task):
  print x.result
