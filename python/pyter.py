#!/usr/bin/python 
import time, timeit

def iter():
  for x in range(0, iterable.count):
    print iterable.get(0)

# iterable[x] ca serait pas mal :) 
# et for i in iterable:
#   print i ca serait encore mieux :)

def pyget(self, args):
  l = [ 'pystring', 'pysecondstring', 'pythirdstring']
  #print l[0]
  return l[0]

from _destruct import *
t = Test()
i = t.getObjectValue()
x = i.get(0)
#print x
#print 'change default function'
#i.get = pyget
#print i.get

#f = i.get
##for x in range(0, 1000000):
  ##ir = i.get(0)
  ##ir = f(0)
  ##ir = i.get((0,1))
##i.get((0, 0))
#print 'Call python from python'

#b = time.time()
#print i.get(0)
##for x in range(0, 1000000):
  ##z = i.get(0)
#a = time.time()
#print 'python to python : ' + str(a-b)
#b = time.time()
#t.setObjectValue(i)
#a = time.time()
#print 'c++ to python : ' + str(a-b)

#print i.instanceOf().name()
class PyIterator(DObject):
  def __init__(self):
    print dir(self)
    DObject.__init__(self, "DVector<String>")
    self.l = [] #XXX class are frozen !!!   because of set /get qui call pas le truc original je suppose 
    #self.current = -1 

  #def __len__(self):
     #print self.count
     #return self.count 

  def __getitem__(self, args):
     if args < self.count:     
       return self.get(args)   
     raise StopIteration()   
  def get(self, args):
     ##print 'python code called'
     return self.l[args] # XXX ralentie a mort meme en fast call car a chaque fois ca fait un getValue / maintennat que les attrib sont SET ET ENLEVE DU DICT :)

  def push(self, args): #XXX pouvoir heriter directe c ds le dico donc gestion de DICO ou inverse get/set call dico
     ##print 'py push'
     self.l.append(args)
     self.count += 1
     return self.count

  def callVoid(self):
     ##print 'python enter callVoid(self)'
     return 120

  def returnVoid(self, args):
     ##print 'python enter returnVoid(self, args) ' + str(args)
     return None

  def allVoid(self):
     ##print 'python enter allVoid(self)' 
     return None
       
class PyPy(object):
  def __init__(self):
    self.l = ['prout', 'prit', 'prot']

  def get(self, args):  
     return self.l[args]

  def callVoid(self, o):
     return 120

  def returnVoid(self, args):
     return None

  def allVoid(self, o):
     return None

COUNT = 1
COUNT = 10**6
#
def timeFunc(func, args, count = COUNT):
   a = time.time()
   for x in xrange(count):
     w = func(args)
   b = time.time()
   print str(func) + " : "+  str(b-a)

pi = PyIterator()
#pi.get = fget

pi.push("pyfirst")
pi.push("pysecond")
pi.push("pythird")
#
##print pi.get(pi)
print pi.get(0)
print pi.get(1)
print pi.count

#pi.get(0)
print "======  CPP Call  ===="
t.setObjectValue(pi)
print "======================"

print "------ Python call c++ -----"

timeFunc(i.get, 1)
timeFunc(i.callVoid, None)
timeFunc(i.allVoid, None)
timeFunc(i.returnVoid, 1)

print "------ Python call python via ds ----"

timeFunc(pi.get, 1)
timeFunc(pi.callVoid, None)
timeFunc(pi.allVoid, None)
timeFunc(pi.returnVoid, 1)

print "----- Real pure python call -----"

pypy = PyPy()
timeFunc(pypy.get, 1)
timeFunc(pypy.callVoid, None)
timeFunc(pypy.allVoid, None)
timeFunc(pypy.returnVoid, 1)

print "------ Python / CPP object func ----"

timeFunc(pi.returnObject, None)
timeFunc(pi.callObject, pi)


#for x in pi:
  #print x

#print "pi.returnVoid(1)"
#print pi.returnVoid(1)
#print "pi.callVoid()"
#print pi.callVoid()
#print "pi.allVoid()"
#print pi.allVoid()
