# DFF -- An Open Source Digital Forensics Framework
# Copyright (C) 2009-2013 ArxSys
# This program is free software, distributed under the terms of
# the GNU General Public License Version 2. See the LICENSE file
# at the top of the source tree.
#  
# See http://www.digital-forensic.org for more information about this
# project. Please do not directly contact any of the maintainers of
# DFF for assistance; the project provides a web site, mailing lists
# and IRC channels for your use.
# 
# Author(s):
#  Solal Jacob <sja@digital-forensic.org>
add_custom_target(pyregistry)
add_dependencies(examples pyregistry)
add_dependencies(_destruct pyregistry)
configure_file(registry.py registry.py COPYONLY)
configure_file(qregistry.py qregistry.py COPYONLY)

set (pydestruct_examples_registry_files
registry.py
qregistry.py
)

