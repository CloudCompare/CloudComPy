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
//#          Copyright 2021 Paul RASCLE www.openfields.fr                  #
//#                                                                        #
//##########################################################################

#include "ccMeshPy.hpp"

#include <boost/python.hpp>

#include <CCGeom.h>
#include <ccMesh.h>
#include <ccGenericPointCloud.h>
#include "ccMeshPy_DocStrings.hpp"

using namespace boost::python;

void export_ccMesh()
{
    class_<CCCoreLib::GenericIndexedMesh, boost::noncopyable>("GenericIndexedMesh", no_init)
        ;

    class_<ccGenericMesh, bases<CCCoreLib::GenericIndexedMesh, ccShiftedObject>, boost::noncopyable>("ccGenericMesh", no_init)
        .def("getName", &ccGenericMesh::getName)
        ;

    class_<ccMesh, bases<ccGenericMesh> >("ccMesh", ccMeshPy_ccMesh_doc, no_init)
        .def("size", &ccMesh::size)
        .def("getAssociatedCloud", &ccMesh::getAssociatedCloud,
             return_value_policy<reference_existing_object>(), ccMeshPy_getAssociatedCloud_doc)
        ;

    //TODO static ccMesh* Triangulate( ccGenericPointCloud* cloud,...
}
