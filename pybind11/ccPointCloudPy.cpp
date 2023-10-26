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

#include "pyccTrace.h"
#include "cloudComPy.hpp"

#include <exception>
#include <Python.h>

#include <ccPointCloud.h>
#include <ccPolyline.h>
#include <ccScalarField.h>
#include <GenericProgressCallback.h>
#include <ccColorScale.h>
#include <ccColorScalesManager.h>
#include <ccColorTypes.h>
#include <ccCommon.h>
#include <ScalarFieldTools.h>
#include <ccSensor.h>
#include <ccGBLSensor.h>
#include <ccHObjectCaster.h>

#include "PyScalarType.h"
#include "ccPointCloudPy_DocStrings.hpp"

#include <map>
#include <QColor>
#include <QString>

struct color_exception : std::exception
{
  const char* what() const noexcept { return "this point cloud has no color table!"; }
};

struct colorSize_exception : std::exception
{
  const char* what() const noexcept { return "the color array has not the same size as this cloud"; }
};

void translate(color_exception const& e)
{
    // Use the Python 'C' API to set up an exception object
    PyErr_SetString(PyExc_RuntimeError, e.what());
}

bool exportCoordToSF_py(ccPointCloud &self, bool x, bool y, bool z)
{
    bool b[3];
    b[0] =x; b[1] = y; b[2] = z;
    return self.exportCoordToSF(b);
}

bool exportNormalToSF_py(ccPointCloud &self, bool x, bool y, bool z)
{
    bool b[3];
    b[0] =x; b[1] = y; b[2] = z;
    return self.exportNormalToSF(b);
}

void coordsFromNPArray_copy(ccPointCloud &self, py::array_t<PointCoordinateType, py::array::c_style | py::array::forcecast> array)
{
    if (array.ndim() != 2)
    {
        throw std::runtime_error("Incorrect array dimension");
    }
    if ( array.shape(1) != 3 )
      throw std::runtime_error("Input should have size [N,2]");
    size_t nRows = array.shape(0);
    self.reserve(nRows);
    self.resize(nRows);
    const PointCoordinateType *s = reinterpret_cast<const PointCoordinateType*>(array.data());
    PointCoordinateType *d = (PointCoordinateType*)self.getPoint(0);
    memcpy(d, s, 3*nRows*sizeof(PointCoordinateType));
    CCTRACE("copied " << 3*nRows*sizeof(PointCoordinateType) << " bytes");
}

void normalsFromNPArray_copy(ccPointCloud &self, py::array_t<PointCoordinateType, py::array::c_style | py::array::forcecast> array)
{
    if (array.ndim() != 2)
    {
        throw std::runtime_error("Incorrect array dimension");
    }
    if ( array.shape(1) != 3 )
      throw std::runtime_error("Input should have size [N,2]");
    size_t nRows = array.shape(0);
    if ( array.shape(0) != self.size() )
        throw std::runtime_error("Input should have size [cloud.size(),2]");
    if (!self.hasNormals())
        self.reserveTheNormsTable();
    const PointCoordinateType *s = reinterpret_cast<const PointCoordinateType*>(array.data());
    for (size_t i=0; i<self.size(); ++i)
    {
        const CCVector3& N = reinterpret_cast<const CCVector3&>(s[3*i]);
        CCVector3 NN = N;
        NN.normalize();
        self.setPointNormal(i, NN);
    }
    CCTRACE("normals: " << 3*nRows*sizeof(PointCoordinateType) << " bytes read, compressed to " << nRows*sizeof(int) << " bytes");
}

bool colorize_py(ccPointCloud &self, float r, float g, float b, float a=1.0f)
{
    bool success = self.colorize(r, g, b, a);
    self.showSF(false);
    self.showColors(true);
    return success;
}

void colorsFromNPArray_copy(ccPointCloud &self, py::array_t<ColorCompType, py::array::c_style | py::array::forcecast> array)
{
    if (array.ndim() != 2)
    {
        throw std::runtime_error("Incorrect array dimension");
    }
    if (array.shape(1) != 4)
    {
        throw std::runtime_error("Incorrect array, 4 components required");
    }
    size_t nRows = array.shape(0);
    if (nRows != self.size())
    {
    	CCTRACE("the color array has not the same size as this cloud!")
		throw colorSize_exception();
    }
    self.resizeTheRGBTable(false);
    if (self.rgbaColors() == nullptr)
    {
    	CCTRACE("no color table in this point cloud!")
		throw color_exception();
    }
    const ColorCompType* s = reinterpret_cast<const ColorCompType*>(array.data());
    ColorCompType* d = (ColorCompType*)(self.rgbaColors()->data());
    memcpy(d, s, 4*nRows*sizeof(ColorCompType));
    CCTRACE("copied " << 4*nRows*sizeof(ColorCompType) << " bytes");
    self.showSF(false);
    self.showColors(true);
    self.colorsHaveChanged();
}

std::map<QString, int> getScalarFieldDic_py(ccPointCloud &self)
{
    std::map<QString, int> mapSF;
    int nbSF = self.getNumberOfScalarFields();
    for (int i=0; i < nbSF; i++)
    {
        mapSF[self.getScalarFieldName(i)] = i;
    }
    return mapSF;
}

CCCoreLib::ScalarField* getScalarFieldByName_py(ccPointCloud &self, const QString& name)
{
    int nbSF = self.getNumberOfScalarFields();
    for (int i=0; i < nbSF; i++)
    {
        if (self.getScalarFieldName(i) == name)
        {
            return self.getScalarField(i);
        }
    }
    return nullptr;
}

py::array CoordsToNpArray_copy(ccPointCloud &self)
{
    CCTRACE("CoordsToNpArray with copy, ownership transfered to Python");
    size_t nRows = self.size();
    PointCoordinateType* s = (PointCoordinateType*) self.getPoint(0);
    size_t ndim = 2;
    std::vector<size_t> shape =
    { nRows, 3 };
    std::vector<size_t> strides =
    { 3 * sizeof(PointCoordinateType), sizeof(PointCoordinateType) };
    return py::array(py::buffer_info(s,                                                    // data as contiguous array
                                     sizeof(PointCoordinateType),                          // size of one scalar
                                     py::format_descriptor<PointCoordinateType>::format(), // data type
                                     ndim,                                                 // number of dimensions
                                     shape,                                                // shape of the matrix
                                     strides                                               // strides for each axis
                                     ));
}

py::array CoordsToNpArray_py(ccPointCloud &self)
{
    CCTRACE("CoordsToNpArray without copy, ownership stays in C++");
    size_t nRows = self.size();
    PointCoordinateType* s = (PointCoordinateType*) self.getPoint(0);
    size_t ndim = 2;
    std::vector<size_t> shape = { nRows, 3 };
    std::vector<size_t> strides = { 3 * sizeof(PointCoordinateType), sizeof(PointCoordinateType) };
    auto capsule = py::capsule(s, [](void *v) { CCTRACE("C++ coords not deleted"); });
    return py::array(py::buffer_info(s,                                                    // data as contiguous array
                                     sizeof(PointCoordinateType),                          // size of one scalar
                                     py::format_descriptor<PointCoordinateType>::format(), // data type
                                     ndim,                                                 // number of dimensions
                                     shape,                                                // shape of the matrix
                                     strides),                                               // strides for each axis
                     capsule);
}

py::array normalsToNpArray_copy(ccPointCloud &self)
{
    CCTRACE("normalsToNpArray with copy, ownership transfered to Python");
    if (!self.hasNormals())
    {
        CCTRACE("This cloud does not have normals!");
        // --- return a minimal nparray of only one value
        std::vector<size_t> shape = { 1 };
        std::vector<size_t> strides = { sizeof(PointCoordinateType) };
        PointCoordinateType* s = (PointCoordinateType*) self.getPoint(0); // any value us OK here, not used
        return py::array(py::buffer_info(s,                                                    // data as contiguous array
                                         sizeof(PointCoordinateType),                          // size of one scalar
                                         py::format_descriptor<PointCoordinateType>::format(), // data type
                                         1,                                                    // number of dimensions
                                         shape,                                                // shape of the matrix
                                         strides                                               // strides for each axis
                                         ));
    }
    size_t ndim = 2;
    size_t nRows = self.size();
    PointCoordinateType *s = new PointCoordinateType[3*nRows]; // array created here, ownership transfered to Python
    for (size_t i=0; i<nRows; ++i)
    {
        const CCVector3& norm = self.getPointNormal(i);
        for(size_t j=0; j<3; ++j) s[3*i+j] = norm[j];
    }
    std::vector<size_t> shape = { nRows, 3 };
    std::vector<size_t> strides = { 3 * sizeof(PointCoordinateType), sizeof(PointCoordinateType) };
    auto capsule = py::capsule(s, [](void *v) { CCTRACE("C++ array not deleted"); });
    return py::array(py::buffer_info(s,                                                    // data as contiguous array
                                     sizeof(PointCoordinateType),                          // size of one scalar
                                     py::format_descriptor<PointCoordinateType>::format(), // data type
                                     ndim,                                                 // number of dimensions
                                     shape,                                                // shape of the matrix
                                     strides),                                             // strides for each axis
                     capsule);
}

py::array ColorsToNpArray_copy(ccPointCloud &self)
{
    CCTRACE("ColorsToNpArray with copy, ownership transfered to Python");
    if (self.rgbaColors() == nullptr)
    {
    	CCTRACE("no color in this point cloud!")
		throw color_exception();
    }
    size_t nRows = self.size();
    ColorCompType* s = (ColorCompType*) (self.rgbaColors()->data());
    size_t ndim = 2;
    std::vector<size_t> shape = { nRows, 4 };
    std::vector<size_t> strides = { 4 * sizeof(ColorCompType), sizeof(ColorCompType) };
    return py::array(py::buffer_info(s,                                              // data as contiguous array
                                     sizeof(ColorCompType),                          // size of one scalar
                                     py::format_descriptor<ColorCompType>::format(), // data type
                                     ndim,                                           // number of dimensions
                                     shape,                                          // shape of the matrix
                                     strides                                         // strides for each axis
                                     ));
}

py::array ColorsToNpArray_py(ccPointCloud &self)
{
    CCTRACE("ColorsToNpArray without copy, ownership stays in C++");
    if (self.rgbaColors() == nullptr)
    {
        CCTRACE("no color in this point cloud!")
        throw color_exception();
    }
    size_t nRows = self.size();
    ColorCompType* s = (ColorCompType*) (self.rgbaColors()->data());
    size_t ndim = 2;
    std::vector<size_t> shape = { nRows, 4 };
    std::vector<size_t> strides = { 4 * sizeof(ColorCompType), sizeof(ColorCompType) };
    auto capsule = py::capsule(s, [](void *v) { CCTRACE("C++ colors not deleted"); });
    return py::array(py::buffer_info(s,                                              // data as contiguous array
                                     sizeof(ColorCompType),                          // size of one scalar
                                     py::format_descriptor<ColorCompType>::format(), // data type
                                     ndim,                                           // number of dimensions
                                     shape,                                          // shape of the matrix
                                     strides                                         // strides for each axis
                                     ),
                     capsule);
}

bool changeColorLevels_py(ccPointCloud &self, unsigned char sin0,
        unsigned char sin1, unsigned char sout0, unsigned char sout1,
        bool onRed, bool onGreen, bool onBlue)
{
    if (self.rgbaColors() == nullptr)
    {
        CCTRACE("no color in this point cloud!")
        throw color_exception();
    }
    if ((sin0 >= sin1) || (sout0 >= sout1))
        return false;

    // --- copied and adapted from ccColorLevelsDlg::onApply()

    bool applyRGB[3] =
    { onRed, onGreen, onBlue };

    unsigned pointCount = self.size();
    int qIn = sin1 - sin0;
    int pOut = sout1 - sout0;
    for (unsigned i = 0; i < pointCount; ++i)
    {
        const ccColor::Rgba &col = self.getPointColor(i);
        ccColor::Rgba newRgb;
        for (unsigned c = 0; c < 3; ++c)
        {
            if (applyRGB[c])
            {
                double newC = sout0;
                if (qIn)
                {
                    double u = (static_cast<double>(col.rgba[c]) - sin0) / qIn;
                    newC = sout0 + u * pOut;
                }
                newRgb.rgba[c] = static_cast<ColorCompType>(std::max<double>(
                        std::min<double>(newC, ccColor::MAX), 0.0));
            }
            else
            {
                newRgb.rgba[c] = col.rgba[c];
            }
        }
        newRgb.a = col.a;

        self.setPointColor(i, newRgb);
    }
    self.showSF(false);
    self.showColors(true);
    return true;
}

bool computeScalarFieldGradient_py( ccPointCloud &self,
                                    int SFindex,
                                    PointCoordinateType radius,
                                    bool euclideanDistances,
                                    CCCoreLib::DgmOctree* theOctree = nullptr)
{
    int nbSF = self.getNumberOfScalarFields();
    if (SFindex < 0 || SFindex >= nbSF)
    {
        CCTRACE("computeScalarFieldGradient: Wrong Scalar Field index!");
        return false;
    }
    self.setCurrentInScalarField(-1);
    self.setCurrentOutScalarField(SFindex);
    QString sfName = QString("%1(%2)").arg(CC_GRADIENT_NORMS_FIELD_NAME, self.getScalarFieldName(SFindex));
    int ret = CCCoreLib::ScalarFieldTools::computeScalarFieldGradient(&self, radius, euclideanDistances,
        false, nullptr, theOctree);
    if (ret != 0)
    {
        CCTRACE("computeScalarFieldGradient: error " << ret);
        return false;
    }
    nbSF = self.getNumberOfScalarFields();
    self.getScalarField(nbSF-1)->computeMinAndMax();
    self.getScalarField(nbSF-1)->setName(sfName.toStdString().c_str());
    return true;
}

bool convertNormalToDipDirSFs_py(ccPointCloud &self)
{
    // --- from ccEntityAction::convertNormalsTo
    //get/create 'dip' scalar field
     int dipSFIndex = self.getScalarFieldIndexByName(CC_DEFAULT_DIP_SF_NAME);
     if (dipSFIndex < 0)
         dipSFIndex = self.addScalarField(CC_DEFAULT_DIP_SF_NAME);
     if (dipSFIndex < 0)
     {
         CCTRACE("[ccEntityAction::convertNormalsTo] Not enough memory!");
         return false;
     }

     //get/create 'dip direction' scalar field
     int dipDirSFIndex = self.getScalarFieldIndexByName(CC_DEFAULT_DIP_DIR_SF_NAME);
     if (dipDirSFIndex < 0)
         dipDirSFIndex = self.addScalarField(CC_DEFAULT_DIP_DIR_SF_NAME);
     if (dipDirSFIndex < 0)
     {
         self.deleteScalarField(dipSFIndex);
         CCTRACE("[ccEntityAction::convertNormalsTo] Not enough memory!");
         return false;
     }

     ccScalarField* dipSF = static_cast<ccScalarField*>(self.getScalarField(dipSFIndex));
     ccScalarField* dipDirSF = static_cast<ccScalarField*>(self.getScalarField(dipDirSFIndex));

     bool success = self.convertNormalToDipDirSFs(dipSF, dipDirSF);

     if (success)
     {
         //apply default 360 degrees color scale!
         ccColorScale::Shared dipScale = ccColorScalesManager::GetDefaultScale(ccColorScalesManager::DIP_BRYW);
         ccColorScale::Shared dipDirScale = ccColorScalesManager::GetDefaultScale(ccColorScalesManager::DIP_DIR_REPEAT);
         dipSF->setColorScale(dipScale);
         dipDirSF->setColorScale(dipDirScale);
         self.setCurrentDisplayedScalarField(dipDirSFIndex); //dip dir. seems more interesting by default
     }
     else
     {
         self.deleteScalarField(dipSFIndex);
         self.deleteScalarField(dipDirSFIndex);
         success = false;
     }
     return success;
}

ccPointCloud* crop2D_py(ccPointCloud &self, const ccPolyline* poly, unsigned char orthoDim, bool inside = true)
{
    ccPointCloud* croppedCloud = nullptr;
    CCTRACE("ortho dim " <<  orthoDim);
    CCCoreLib::ReferenceCloud* ref = self.crop2D(poly, orthoDim, inside);
    if (ref && (ref->size() != 0))
    {
        croppedCloud = self.partialClone(ref);
        delete ref;
        ref = nullptr;
    }
    return croppedCloud;
}

void fuse_py(ccPointCloud &self, ccPointCloud* other)
{
    self += other;
}

bool interpolateColorsFrom_py(ccPointCloud &self, ccGenericPointCloud* otherCloud, unsigned char octreeLevel = 0)
{
    if (!otherCloud || otherCloud->size() == 0)
    {
        CCTRACE("Invalid/empty input cloud!");
        return false;
    }
    if (!otherCloud->hasColors())
    {
        CCTRACE("input cloud has no color");
        return false;
    }
    self.showSF(false);
    self.showColors(true);
    return self.interpolateColorsFrom(otherCloud, nullptr, octreeLevel);
}

bool orientNormalsWithFM_py(ccPointCloud &self, unsigned char octreeLevel = 6)
{
    return self.orientNormalsWithFM(octreeLevel);
}

bool orientNormalsWithMST_py(ccPointCloud &self, unsigned char octreeLevel = 6)
{
    return self.orientNormalsWithMST(octreeLevel);
}

py::tuple partialClone_py(ccPointCloud &self,
                          const CCCoreLib::ReferenceCloud* selection)
{
    int warnings;
    ccPointCloud* cloud = self.partialClone(selection, &warnings);
    py::tuple res = py::make_tuple(cloud, warnings);
    return res;
}

bool setColor_py(ccPointCloud &self, QColor unique)
{
	ccColor::Rgba col = ccColor::FromQColora(unique);
    bool success = self.setColor(col);
    self.showSF(false);
    self.showColors(true);
	return success;
}

bool setColorGradientDefault_py(ccPointCloud &self, unsigned char heightDim)
{
    ccColorScale::Shared colorScale(nullptr);
    colorScale = ccColorScalesManager::GetDefaultScale();
    bool success = self.setRGBColorByHeight(heightDim, colorScale);
    self.showSF(false);
    self.showColors(true);
    return success;
}

bool setColorGradient_py(ccPointCloud &self, unsigned char heightDim, QColor first, QColor second)
{
    ccColorScale::Shared colorScale(nullptr);
    colorScale = ccColorScale::Create("Temp scale");
    colorScale->insert(ccColorScaleElement(0.0, first), false);
    colorScale->insert(ccColorScaleElement(1.0, second), true);
    bool success = self.setRGBColorByHeight(heightDim, colorScale);
    self.showSF(false);
    self.showColors(true);
    return success;
}

bool setColorGradientBanded_py(ccPointCloud &self, unsigned char heightDim, double frequency)
{
    bool success = self.setRGBColorByBanding(heightDim, frequency);
    self.showSF(false);
    self.showColors(true);
    return success;
}

QString GetFirstAvailableSFName(const ccPointCloud &cloud, const QString &baseName)
{
    // --- adapted from ccEntityAction::GetFirstAvailableSFName
    QString name = baseName;
    int tries = 0;
    while (cloud.getScalarFieldIndexByName(qPrintable(name)) >= 0 || tries > 99)
        name = QString("%1 #%2").arg(baseName).arg(++tries);
    if (tries > 99)
        return QString();
    return name;
}

bool sfFromColor_py(ccPointCloud &self, bool exportR, bool exportG, bool exportB, bool exportAlpha, bool exportComposite)
{
    // --- adapted from ccEntityAction::sfFromColor
    std::vector<ccScalarField*> fields(5, nullptr);
    fields[0] = (exportR ? new ccScalarField(qPrintable(GetFirstAvailableSFName(self, "R"))) : nullptr);
    fields[1] = (exportG ? new ccScalarField(qPrintable(GetFirstAvailableSFName(self, "G"))) : nullptr);
    fields[2] = (exportB ? new ccScalarField(qPrintable(GetFirstAvailableSFName(self, "B"))) : nullptr);
    fields[3] = (exportAlpha ? new ccScalarField(qPrintable(GetFirstAvailableSFName(self, "Alpha"))) : nullptr);
    fields[4] = (exportComposite ? new ccScalarField(qPrintable(GetFirstAvailableSFName(self, "Composite"))) : nullptr);

    //try to instantiate memory for each field
    unsigned count = self.size();
    for (ccScalarField *&sf : fields)
    {
        if (sf && !sf->reserveSafe(count))
        {
            sf->release();
            sf = nullptr;
        }
    }

    //export points
    for (unsigned j = 0; j < self.size(); ++j)
    {
        const ccColor::Rgba &col = self.getPointColor(j);

        if (fields[0])
            fields[0]->addElement(col.r);
        if (fields[1])
            fields[1]->addElement(col.g);
        if (fields[2])
            fields[2]->addElement(col.b);
        if (fields[3])
            fields[3]->addElement(col.a);
        if (fields[4])
            fields[4]->addElement(static_cast<ScalarType>(col.r + col.g + col.b) / 3);
    }

    QString fieldsStr;

    for (ccScalarField *&sf : fields)
    {
        if (sf == nullptr)
            continue;

        sf->computeMinAndMax();

        int sfIdx = self.getScalarFieldIndexByName(sf->getName());
        if (sfIdx >= 0)
            self.deleteScalarField(sfIdx);
        sfIdx = self.addScalarField(sf);
        if (sfIdx < 0)
        {
            sf->release();
            sf = nullptr;
        }
    }
    return true;
}

std::vector<ccSensor*> getSensors(ccPointCloud &self)
{
    std::vector<ccSensor*> sensors;
    for (unsigned i = 0; i < self.getChildrenNumber(); ++i)
    {
        ccHObject* child = self.getChild(i);
        if (child && child->isKindOf(CC_TYPES::SENSOR))
        {
            if (child->isKindOf(CC_TYPES::GBL_SENSOR))
            {
                ccGBLSensor* sensor = ccHObjectCaster::ToGBLSensor(child);
                sensors.push_back(sensor);
                CCTRACE("ccGBLSensor found")
            }
            else
            {
                ccSensor* sensor = ccHObjectCaster::ToSensor(child);
                sensors.push_back(sensor);
                CCTRACE("ccSensor found")
            }
        }
    }
    return sensors;
}

void applyRigidTransformationPy(ccPointCloud &self, const ccGLMatrixTpl<float>& m)
{
    //ccGLMatrix* matrix = m;
    CCTRACE("applyRigidTransformationPy");
    self.applyRigidTransformation(m);
}

int (ccPointCloud::*addScalarFieldt)(const char*) = &ccPointCloud::addScalarField;

void export_ccPointCloud(py::module &m0)
{
    py::enum_<ccPointCloud::CLONE_WARNINGS>(m0, "CLONE_WARNINGS")
        .value("WRN_OUT_OF_MEM_FOR_COLORS", ccPointCloud::CLONE_WARNINGS::WRN_OUT_OF_MEM_FOR_COLORS)
        .value("WRN_OUT_OF_MEM_FOR_NORMALS", ccPointCloud::CLONE_WARNINGS::WRN_OUT_OF_MEM_FOR_NORMALS)
        .value("WRN_OUT_OF_MEM_FOR_SFS", ccPointCloud::CLONE_WARNINGS::WRN_OUT_OF_MEM_FOR_SFS)
        .value("WRN_OUT_OF_MEM_FOR_FWF", ccPointCloud::CLONE_WARNINGS::WRN_OUT_OF_MEM_FOR_FWF)
        .export_values();

    py::class_<ccPointCloud, CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>,
               std::unique_ptr<ccPointCloud, py::nodelete> >(m0, "ccPointCloud", ccPointCloudPy_ccPointCloud_doc)
        .def(py::init<QString, unsigned>(), py::arg("name")=QString(), py::arg("uniqueID")=0xFFFFFFFF) // TODO optional<QString, unsigned> >())
        .def("addScalarField", addScalarFieldt, ccPointCloudPy_addScalarField_doc)
        .def("applyRigidTransformation", &ccPointCloud::applyRigidTransformation, ccPointCloudPy_applyRigidTransformation_doc)
        .def("applyRigidTransformation", &applyRigidTransformationPy, ccPointCloudPy_applyRigidTransformation_doc)
        .def("cloneThis", &ccPointCloud::cloneThis,
             py::arg("destCloud")=nullptr, py::arg("ignoreChildren")=false,
             ccPointCloudPy_cloneThis_doc, py::return_value_policy::reference)
        .def("changeColorLevels", &changeColorLevels_py, ccPointCloudPy_changeColorLevels_doc)
        .def("colorize", &colorize_py, py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a")=1.0f,
         ccPointCloudPy_colorize_doc)
        .def("computeGravityCenter", &ccPointCloud::computeGravityCenter, ccPointCloudPy_computeGravityCenter_doc)
        .def("computeScalarFieldGradient", &computeScalarFieldGradient_py,
             py::arg("SFindex"), py::arg("radius"), py::arg("euclideanDistances"),
             py::arg("theOctree")=nullptr, ccPointCloudPy_computeScalarFieldGradient_doc)
        .def("colorsFromNPArray_copy", &colorsFromNPArray_copy, ccPointCloudPy_colorsFromNPArray_copy_doc)
        .def("coordsFromNPArray_copy", &coordsFromNPArray_copy, ccPointCloudPy_coordsFromNPArray_copy_doc)
        .def("convertCurrentScalarFieldToColors", &ccPointCloud::convertCurrentScalarFieldToColors,
             py::arg("mixWithExistingColor")=false,
             ccPointCloudPy_convertCurrentScalarFieldToColors_doc)
        .def("convertNormalToRGB", &ccPointCloud::convertNormalToRGB, ccPointCloudPy_convertNormalToRGB_doc)
        .def("convertNormalToDipDirSFs", convertNormalToDipDirSFs_py, ccPointCloudPy_convertNormalToDipDirSFs_doc)
        .def("convertRGBToGreyScale", &ccPointCloud::convertRGBToGreyScale, ccPointCloudPy_convertRGBToGreyScale_doc)
        .def("crop2D", &crop2D_py, py::return_value_policy::reference, ccPointCloudPy_crop2D_doc)
        .def("deleteAllScalarFields", &ccPointCloud::deleteAllScalarFields, ccPointCloudPy_deleteAllScalarFields_doc)
        .def("deleteScalarField", &ccPointCloud::deleteScalarField, ccPointCloudPy_deleteScalarField_doc)
        .def("enhanceRGBWithIntensitySF", &ccPointCloud::enhanceRGBWithIntensitySF,
             py::arg("sfIdx"), py::arg("useCustomIntensityRange")=false, py::arg("minI")=0.0, py::arg("maxI")=1.0,
             ccPointCloudPy_enhanceRGBWithIntensitySF_doc)
        .def("exportCoordToSF", &exportCoordToSF_py, ccPointCloudPy_exportCoordToSF_doc)
        .def("exportNormalToSF", &exportNormalToSF_py, ccPointCloudPy_exportNormalToSF_doc)
        .def("filterPointsByScalarValue", &ccPointCloud::filterPointsByScalarValue,
             py::arg("minVal"), py::arg("maxVal"), py::arg("outside")=false,
             ccPointCloudPy_filterPointsByScalarValue_doc, py::return_value_policy::reference)
        .def("fuse", &fuse_py, ccPointCloudPy_fuse_doc)
        .def("getCurrentDisplayedScalarField", &ccPointCloud::getCurrentDisplayedScalarField,
             py::return_value_policy::reference, ccPointCloudPy_getCurrentDisplayedScalarField_doc)
        .def("getCurrentDisplayedScalarFieldIndex", &ccPointCloud::getCurrentDisplayedScalarFieldIndex,
             ccPointCloudPy_getCurrentDisplayedScalarFieldIndex_doc)
        .def("getCurrentInScalarField", &ccPointCloud::getCurrentInScalarField,
             py::return_value_policy::reference, ccPointCloudPy_getCurrentInScalarField_doc)
        .def("getCurrentOutScalarField", &ccPointCloud::getCurrentOutScalarField,
             py::return_value_policy::reference, ccPointCloudPy_getCurrentOutScalarField_doc)
        .def("getNumberOfScalarFields", &ccPointCloud::getNumberOfScalarFields, ccPointCloudPy_getNumberOfScalarFields_doc)
        .def("getScalarField", &ccPointCloud::getScalarField,
             py::return_value_policy::reference, ccPointCloudPy_getScalarField_doc)
        .def("getScalarField", &getScalarFieldByName_py,
             py::return_value_policy::reference, ccPointCloudPy_getScalarFieldByName_doc)
        .def("getScalarFieldDic", &getScalarFieldDic_py, ccPointCloudPy_getScalarFieldDic_doc)
        .def("getScalarFieldName", &ccPointCloud::getScalarFieldName, ccPointCloudPy_getScalarFieldName_doc)
        .def("getSensors", getSensors, ccPointCloudPy_getSensors_doc)
        .def("hasColors", &ccPointCloud::hasColors, ccPointCloudPy_hasColors_doc)
        .def("hasNormals", &ccPointCloud::hasNormals, ccPointCloudPy_hasNormals_doc)
        .def("hasScalarFields", &ccPointCloud::hasScalarFields, ccPointCloudPy_hasScalarFields_doc)
        .def("interpolateColorsFrom", &interpolateColorsFrom_py,
             py::arg("otherCloud"), py::arg("octreeLevel")=0,
             ccPointCloudPy_interpolateColorsFrom_doc)
        .def("normalsFromNpArrayCopy", &normalsFromNPArray_copy, ccPointCloudPy_normalsFromNpArrayCopy_doc)
        .def("normalsToNpArrayCopy", &normalsToNpArray_copy, ccPointCloudPy_normalsToNpArrayCopy_doc)
        .def("orientNormalsWithFM", orientNormalsWithFM_py,
             py::arg("octreeLevel")=6,
             ccPointCloudPy_orientNormalsWithFM_doc)
        .def("orientNormalsWithMST", &orientNormalsWithMST_py,
             py::arg("octreeLevel")=6,
             ccPointCloudPy_orientNormalsWithMST_doc)
        .def("partialClone", &partialClone_py, ccPointCloudPy_partialClone_doc)
        .def("renameScalarField", &ccPointCloud::renameScalarField, ccPointCloudPy_renameScalarField_doc)
        .def("reserve", &ccPointCloud::reserve, ccPointCloudPy_reserve_doc)
        .def("resize", &ccPointCloud::resize, ccPointCloudPy_resize_doc)
//        .def("scale",
//             [](ccPointCloud& self, PointCoordinateType fx, PointCoordinateType fy, PointCoordinateType fz, CCVector3 center=CCVector3(0,0,0))
//             {
//                return self.scale(fx, fy, fz, center);
//             },
//             py::arg("fx"), py::arg("fy"), py::arg("fz"), py::arg("center"), //=CCVector3(0,0,0),
//             ccPointCloudPy_scale_doc)
        .def("scale",
             &ccPointCloud::scale,
             py::arg("fx"), py::arg("fy"), py::arg("fz"), py::arg("center") = CCVector3(0,0,0),
             ccPointCloudPy_scale_doc)
        .def("setColor", &setColor_py, ccPointCloudPy_setColor_doc)
        .def("setColorGradient", &setColorGradient_py, ccPointCloudPy_setColorGradient_doc)
        .def("setColorGradientBanded", &setColorGradientBanded_py, ccPointCloudPy_setColorGradientBanded_doc)
        .def("setColorGradientDefault", &setColorGradientDefault_py, ccPointCloudPy_setColorGradientDefault_doc)
        .def("setCurrentDisplayedScalarField", &ccPointCloud::setCurrentDisplayedScalarField,
             ccPointCloudPy_setCurrentDisplayedScalarField_doc)
        .def("setCurrentScalarField", &ccPointCloud::setCurrentScalarField, ccPointCloudPy_setCurrentScalarField_doc)
        .def("setCurrentInScalarField", &ccPointCloud::setCurrentInScalarField, ccPointCloudPy_setCurrentInScalarField_doc)
        .def("setCurrentOutScalarField", &ccPointCloud::setCurrentOutScalarField, ccPointCloudPy_setCurrentOutScalarField_doc)
        .def("sfFromColor", sfFromColor_py, ccPointCloudPy_sfFromColor_doc)
        .def("showSFColorsScale", &ccPointCloud::showSFColorsScale, ccPointCloudPy_showSFColorsScale_doc)
        .def("size", &ccPointCloud::size, ccPointCloudPy_size_doc)
        .def("shrinkToFit", &ccPointCloud::shrinkToFit, ccPointCloudPy_shrinkToFit_doc)
        .def("toNpArray", &CoordsToNpArray_py, ccPointCloudPy_toNpArray_doc)
        .def("toNpArrayCopy", &CoordsToNpArray_copy, ccPointCloudPy_toNpArrayCopy_doc)
        .def("colorsToNpArray", &ColorsToNpArray_py, ccPointCloudPy_colorsToNpArray_doc)
        .def("colorsToNpArrayCopy", &ColorsToNpArray_copy, ccPointCloudPy_colorsToNpArrayCopy_doc)
        .def("translate", &ccPointCloud::translate, ccPointCloudPy_translate_doc)
        .def("unallocateColors", &ccPointCloud::unallocateColors, ccPointCloudPy_unallocateColors_doc)
        .def("unallocateNorms", &ccPointCloud::unallocateNorms, ccPointCloudPy_unallocateNorms_doc)
       ;
}

