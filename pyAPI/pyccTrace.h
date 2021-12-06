//##########################################################################
//#                                                                        #
//#                               CC                                       #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU Library General Public License as       #
//#  published by the Free Software Foundation; version 2 or later of the  #
//#  License.                                                              #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          Copyright 2020 Paul RASCLE www.openfields.fr                  #
//#                                                                        #
//##########################################################################

#ifndef CLOUDCOMPY_PYAPI_CCTRACE_H_
#define CLOUDCOMPY_PYAPI_CCTRACE_H_

#ifdef _PYTHONAPI_DEBUG_
#include <iostream>
#include <sstream>

struct ccTrace
{
static bool _isTrace;
static void settrace();
};

#define CCTRACE(msg) {if (ccTrace::_isTrace) std::cerr<<std::flush<<__FILE__<<" ["<<__LINE__<<"] : "<<msg<<std::endl<<std::flush;}
#else
#define CCTRACE(msg)
#endif

#endif /* CLOUDCOMPY_PYAPI_CCTRACE_H_ */
