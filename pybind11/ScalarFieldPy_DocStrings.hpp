//##########################################################################
//#                                                                        #
//#                              CloudComPy                                #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; either version 3 of the License, or     #
//#  any later version.                                                    #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#  You should have received a copy of the GNU General Public License     #
//#  along with this program. If not, see <https://www.gnu.org/licenses/>. #
//#                                                                        #
//#          Copyright 2020-2021 Paul RASCLE www.openfields.fr             #
//#                                                                        #
//##########################################################################

#ifndef SCALARFIELDPY_DOCSTRINGS_HPP_
#define SCALARFIELDPY_DOCSTRINGS_HPP_

const char* ScalarFieldPy_ScalarField_doc= R"(
A simple scalar field (to be associated to a point cloud).

A monodimensional array of scalar values.
Invalid values can be represented by CCCoreLib::NAN_VALUE.)";

const char* ScalarFieldPy_addElement_doc= R"(
Add a value at the end of the vector.

:param float value: the value to add )";

const char* ScalarFieldPy_computeMeanAndVariance_doc= R"(
Returns a tuple (mean, variance).

:return: (mean, variance)
:rtype: tuple )";

const char* ScalarFieldPy_computeMinAndMax_doc= R"(
Should be called before getMin or getMax, for instance after modifying
the scalar field with Numpy.

No return!)";

const char* ScalarFieldPy_currentSize_doc= R"(
Returns the vector size.

:return: the vector size
:rtype: int )";

const char* ScalarFieldPy_fill_doc= R"(
Fills the array with a particular value.

:param float value: the value to use to fill the array )";

const char* ScalarFieldPy_flagValueAsInvalid_doc= R"(
Sets the value as 'invalid' (i.e. CCCoreLib::NAN_VALUE).

**No check on index validity!**

:param int index: the index in the ScalarField to set as 'invalid' )";

const char* ScalarFieldPy_fromNpArrayCopy_doc= R"(
Copy data from numpy array into an existing ScalarField.

Checks if the numpy array is of PyScalarType, one dimension, same size as the ScalarField,
before overwriting ScalarField data.

:param ndarray nparray: the data source to use to overwrite the ScalarField )";

const char* ScalarFieldPy_getMax_doc= R"(
Returns maximal value of the ScalarField.

You should call computeMinAndMax before, after a modification of the ScalarField with Numpy!

:return: max value
:rtype: float )";

const char* ScalarFieldPy_getMin_doc= R"(
Returns minimal value of the ScalarField.

You should call call computeMinAndMax before, after a modification of the ScalarField with Numpy!

:return: min value
:rtype: float )";

const char* ScalarFieldPy_getName_doc= R"(
Returns the name of the ScalarField.

:return: ScalarField name
:rtype: str )";

const char* ScalarFieldPy_getValue_doc= R"(
Returns the ScalarField value at a given index.

**No check on index validity!**

:param int index: the index

:return: value at index
:rtype: float )";

const char* ScalarFieldPy_reserveSafe_doc= R"(
Reserves memory (no exception thrown).

:param int count: number of elements to reserve in the ScalarField

:return: success
:rtype: bool)";

const char* ScalarFieldPy_resizeSafe_doc= R"(
Resizes memory (no exception thrown).

:param int count: number of elements to keep/reserve in the ScalarField
:param bool init: whether to initialize new elements
:param float value: value for new elements

:return: success
:rtype: bool)";

const char* ScalarFieldPy_setName_doc= R"(
Sets the name of the ScalarField. No control is done on the uniqueness of the name.
It is your responsability to ensure this uniqueness.

:param str name: new name for the ScalarField )";

const char* ScalarFieldPy_setValue_doc= R"(
Set the value at the given index.

**No check on index validity!**

:param int index: index in the ScalarField
:param float value: value to set at index )";

const char* ScalarFieldPy_swap_doc= R"(
Swap values between two indices.

**No check on indices validity!**

:param int index1: first index
:param int index2: second index )";

const char* ScalarFieldPy_toNpArray_doc= R"(
Wrap the ScalarField data into a numpy Array.

Returns a numpy array: a one dimension array of (number of Points).
Data is not copied, the numpy array object does not own the data.

**WARNING** No automatic action on the Python side on the variables referencing the C++ object in case of destruction!

:return: numpy Array pointing to the ScalarField data
:rtype: ndarray
)";

const char* ScalarFieldPy_toNpArrayCopy_doc= R"(
Wrap the ScalarField data into a numpy Array.

Returns a numpy array: a one dimension array of (number of Points).
Data is copied, the numpy array object owns its data.
Ownership is transfered to Python:
the numpy array object and its data will be handled by the Python Garbage Collector.

:return: numpy Array with data copied from the ScalarField data
:rtype: ndarray
)";

const char* ccScalarFieldPy_ccScalarField_doc= R"(
A scalar field associated to display-related parameters.
Extends the CCCoreLib::ScalarField object.)";

const char* ccScalarFieldPy_getGlobalShift_doc= R"(
Returns Global Shift if any.
Used for instance for LAS GPS time shift

:return: global shift
:rtype: float

)";

#endif /* SCALARFIELDPY_DOCSTRINGS_HPP_ */
