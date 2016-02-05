#!/usr/bin/python -i 
import sys

sys.path.append("../")
sys.path.append("../../")

from _destruct import *

def workPool():
  workerPool = DStructs().find("WorkerPool").newObject(DUInt8(4))
  taskStruct = DStructs().find("Task")
  for x in range(0, 4): 
    task = taskStruct.newObject()
    task.function = finder.keyName
    task.argument = DUInt64(x) 
    workerPool.addTask(task)

  workerPool.join()

def serve():
  server = DStructs().find("Server").newObject(DUInt32(3583))
  registry = DStructs().find("Registry").newObject()
  server.setRoot(registry)
  server.daemonize()
 
def load():
  loader = DStructs().find("Import").newObject()
  paths = ["/home/vertrex/destruct/examples/modules/libdestruct_test.so",
           "/home/vertrex/destruct/examples/modules/libregistry.so",
           "/home/vertrex/destruct/examples/modules/libdthreading.so",
           "/home/vertrex/destruct/examples/modules/libdestruct_rpc.so"]
  for path in paths:
    loader.file(path)

 
if __name__ == "__main__":
  load()
  serve()
  #obj = connect()
