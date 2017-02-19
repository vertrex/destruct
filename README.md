# Destruct
Destruct is a dynamic serialization &amp; deserialization lib in c++98 with python 2.7 binding based on an object protocol.

# Features

- C++ 98 Meta object protocol (meta-class & reflection)
- Dynamic structure (meta-class) loaded at run-time
- No code generation 
- No IDL
- Cross language polymorphism 
- Reference counting

## Primitive type  :

- Int  8/16/32/64
- UInt 8/16/32/64
- Unicode string
- Base Object / Dynamic object / Simple Object 
- Method/Function 
- None
- Struct
- Buffer 
- Opaque 
- Unknown

## Default protocol : 

- Container : vector / map
- Iterator

## Embedded objects

- Mutable object
- Mutable struct
- CPP Object / CPP Mutable object
- Serializer / Deserializer : binary / raw / xml / text
- Stream : file / cout / string 
- Debug/Trace : Trace object / Recursive trace object
- Import : C++ dynamic Struct/Object loader 

# Examples & objects

## C++

- DB : serialization / deserialization test
- Device : Stream interface for computer devices 
- Memdevice : Stream interface for Windows RAM 
- Inheritance : test c++ inheritance & composition
- Loader : test importer 
- Perf : performance test (loop)
- Refcount : refcount test
- RPC : Destruct RPC client/server interface
- RPC ZMQ : Destruct RPC client/server interface using ZMQ lib for message passing
- Threading : Thread Safe Object interface, Recursive Thread Safe Object, Worker pool (map), Queue, Task 
- Registry : Windows registry parser 

## Python examples

- Debug : trace object & recursive trace object test
- Device : device test
- Embedding : python embedding
- Inheritance : test cross language polymorphism (C++ inherit python destruct object, python inherit C++ destruct object)
- Iterator : test iterator 
- Loader : test import 
- Performance : test perf
- Registry : registry.py Read distant or local registry, serialize it... qregistry display local or distant reg trought qt GUI
- Serialization : serialization / deserialization test
- Swig : Destruct swig python interface to be able to transparently "use" destruct python binding trough swig wrapped code
- Workerpool : Threading test

# License 

GPLV-2 


