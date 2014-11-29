#!/usr/bin/python

import sys
sys.path.append('../../')

from _destruct import *

print "== create dvector string"
vector = Destruct().find("DVectorString").newObject()
print "== pass it to devctor object"
debugVector = Destruct().find("DebugObject").newObject(vector)

print "== push string and iterate"
print "== push first string"
debugVector.push("first string")
print "== push second string"
debugVector.push("second string")
print "== access index 0"
debugVector[0]
print "== iterator on debugVector"
for string in debugVector:
  print string

print '== base vector is a pointer must iterate on same object'
print "== access index 0"
print vector[0]
vector.push("third string")
for string in vector:
  print  string


print "== create DVectorObject with two object"
origObj = Destruct().find("DVectorObject").newObject()
objects = Destruct().find("DebugObject").newObject(origObj)
print "=========", objects

mapString = Destruct().find("DMapString").newObject()
mapString["first"] = "first string"
mapLong = Destruct().find("DMapUInt64Object").newObject()
mapLong[0] = mapString
objects.push(mapString)
objects.push(mapLong)

print 'obj in ', objects
for obj in objects:
  print 'sub obj in ', obj
  for subObj in obj:
    print '  ', subObj.index ," : ", subObj.value

print "==============recursive debug"
objects = Destruct().find("RecursiveDebugObject").newObject(origObj)

print 'obj in ', objects
for obj in objects:
  o =  obj
  for subObj in obj:
    so = subObj
    soi = subObj.index 
    sov = subObj.value
