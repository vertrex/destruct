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

#include "common.hpp"
#include "performance.hpp"

extern "C"
{
  EXPORT void DestructExport()
  {
    Destruct::DStructs& destruct = Destruct::DStructs::instance();
    registerDCpp(Performance)
  }
}

Performance::Performance(DStruct* dstruct, const DValue& args) : DCppObject<Performance>(dstruct)
{
  this->init();
}

Performance::Performance(Performance const& rhs) : DCppObject<Performance>(rhs)
{
  this->copy(this, rhs);
}

Performance::~Performance()
{
}

DUnicodeString        Performance::callStr(DValue const& args)
{
  return (args);
}
