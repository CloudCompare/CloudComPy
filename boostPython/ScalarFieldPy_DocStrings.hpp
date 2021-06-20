//##########################################################################
//#                                                                        #
//#                                boost.Python                            #
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
//#          Copyright 2021 Paul RASCLE www.openfields.fr                  #
//#                                                                        #
//##########################################################################

#ifndef SCALARFIELDPY_DOCSTRINGS_HPP_
#define SCALARFIELDPY_DOCSTRINGS_HPP_

const char* ScalarFieldPy_ScalarField_doc= R"(
A simple scalar field (to be associated to a point cloud)
A monodimensional array of scalar values.
Invalid values can be represented by CCCoreLib::NAN_VALUE.)";

const char* ScalarFieldPy_addElement_doc= R"(
add a value at the end of the vector)";

const char* ScalarFieldPy_computeMeanAndVariance_doc= R"(
Return a tuple (mean, variance))";

const char* ScalarFieldPy_computeMinAndMax_doc= R"(
Should be called before getMin or getMax, for instance after modifying
the scalar field with Numpy.
No return!)";
const char* ScalarFieldPy_currentSize_doc= R"(
Returns the vector size)";

const char* ScalarFieldPy_fill_doc= R"(
Fills the array with a particular value)";

const char* ScalarFieldPy_flagValueAsInvalid_doc= R"(
Sets the value as 'invalid' (i.e. CCCoreLib::NAN_VALUE))";

const char* ScalarFieldPy_fromNpArrayCopy_doc= R"(
Copy data from numpy array into an existing ScalarField.
Check if the numpy array is of PyScalarType, one dimension, same size as the ScalarField,
before overwriting ScalarField data.)";

const char* ScalarFieldPy_getMax_doc= R"(
Return maximal value of the ScalarField.
Call computeMinAndMax before, after a modification of the ScalarField with Numpy!)";

const char* ScalarFieldPy_getMin_doc= R"(
Return minimal value of the ScalarField.
Call computeMinAndMax before, after a modification of the ScalarField with Numpy!)";

const char* ScalarFieldPy_getName_doc= R"(
return the name of the ScalarField)";

const char* ScalarFieldPy_getValue_doc= R"(
Returns the ScalarField value at a given index)";

const char* ScalarFieldPy_reserveSafe_doc= R"(
Reserves memory (no exception thrown)
param count)";

const char* ScalarFieldPy_resizeSafe_doc= R"(
Resizes memory (no exception thrown))";

const char* ScalarFieldPy_setName_doc= R"(
Sets the name of the ScalarField
param name)";

const char* ScalarFieldPy_setValue_doc= R"(
Set the value at the given index
param index
param value)";

const char* ScalarFieldPy_swap_doc= R"(
Swap values between two indices
param index1
param index2)";

const char* ScalarFieldPy_toNpArray_doc= R"(
Wrap the ScalarField data into a numpy Array.
returns PyArray: a one dimension array of (number of Points)
Data is not copied, the PyArray object does not own the data.
)";

const char* ScalarFieldPy_toNpArrayCopy_doc= R"(
Wrap the ScalarField data into a numpy Array.
returns PyArray: a one dimension array of (number of Points)
Data is copied, the PyArray object owns its data.
Ownership is transfered to Python:
the PyArray object and its data will be handled by the Python Garbage Collector
)";

#endif /* SCALARFIELDPY_DOCSTRINGS_HPP_ */
