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

#ifndef CLOUDCOMPY_HPP_
#define CLOUDCOMPY_HPP_

#define PYBIND11_DETAILED_ERROR_MESSAGES
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h>
#include "conversions.hpp"
#include <QSharedPointer>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, QSharedPointer<T>);
namespace PYBIND11_NAMESPACE { namespace detail {
    template <typename T>
    struct holder_helper<QSharedPointer<T>> { // <-- specialization
        static const T *get(const QSharedPointer<T> &p) { CCTRACE("pointer " << p.data()); return p.data(); }
    };
}}

void export_colors(py::module &);
void export_ScalarField(py::module &);
void export_ccGenericCloud(py::module &);
void export_ccPolyline(py::module &);
void export_ccOctree(py::module &);
void export_ccPointCloud(py::module &);
void export_ccMesh(py::module &);
void export_ccPrimitives(py::module &);
void export_distanceComputationTools(py::module &);
void export_geometricalAnalysisTools(py::module &);
void export_registrationTools(py::module &);
void export_cloudSamplingTools(py::module &);
void export_ccFacet(py::module &);
void export_ccSensor(py::module &);
void export_Neighbourhood(py::module &);

#endif
