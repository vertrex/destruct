/*
 * DFF -- An Open Source Digital Forensics Framework
 * Copyright (C) 2009-2011 ArxSys
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 *  
 * See http: *www.digital-forensic.org for more information about this
 * project. Please do not directly contact any of the maintainers of
 * DFF for assistance; the project provides a web site, mailing lists
 * and IRC channels for your use.
 * 
 * Author(s):
 *  Solal Jacob <sja@digital-forensic.org>
 */

#ifndef __registry_hpp__
#define __registry_hpp__

#include "registry_common.hpp"

class RegistryOpt;
class Regf;

using namespace Destruct;

class Registry : public Destruct::DCppObject<Registry>
{
public:
  static void           declare(void);
  Registry(DStruct* dstruct, const Destruct::DValue&);
  Registry(Registry const&);
  ~Registry();

  Destruct::DValue      open(DValue const& args);
  //void                setStateInfo(const std::string&);
  bool                  load(Destruct::DValue value);
  Destruct::DValue      save(void) const;

  static void           show(DObject* object);
  static void           toFile(std::string fileName, DObject* object, std::string type);

  attributeCount(Registry, 1)

  attributeList(
                function(DObject, open, DUnicodeString)
               )

  memberList(Registry, 
             method(Registry, open)
            )
private:
  Destruct::DStructs&           __destruct;
  RealValue<DFunctionObject*>   _open;
};

#endif
