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

#ifndef __PERF_HPP__
#define __PERF_HPP__

#include "common.hpp"

using namespace Destruct;

class Perf 
{
public:
 Perf();
 ~Perf();

 DObject*       local(void);
 DObject*       connect(std::string const& addr, uint32_t port);
 void           stats(DObject* perf);

 const std::string usage(void) const;
};

#endif
