//##########################################################################
//#                                                                        #
//#                                PYCC                                    #
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

#ifndef CLOUDCOMPY_PYAPI_PYSCALARTYPE_H_
#define CLOUDCOMPY_PYAPI_PYSCALARTYPE_H_

#if defined SCALAR_TYPE_FLOAT
    #define PyScalarType float
    #define CC_NPY_FLOAT NPY_FLOAT32
    #define CC_NPY_FLOAT_STRING "float32"
#else
    #define PyScalarType double
    #define CC_NPY_FLOAT NPY_FLOAT64
    #define CC_NPY_FLOAT_STRING "float64"
#endif

#endif /* CLOUDCOMPY_PYAPI_PYSCALARTYPE_H_ */
