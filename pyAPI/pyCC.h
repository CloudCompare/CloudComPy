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

#ifndef CLOUDCOMPY_PYAPI_PYCC_H_
#define CLOUDCOMPY_PYAPI_PYCC_H_

#include <QString>
#include <vector>

#ifndef SCALAR_TYPE_DOUBLE
  #ifndef SCALAR_TYPE_FLOAT
    #define SCALAR_TYPE_FLOAT
  #endif
#endif

#include <GeometricalAnalysisTools.h>
#include <ccPolyline.h>
#include <ccPointCloud.h>
#include <RegistrationTools.h>
#include <ccNormalVectors.h>
#include <ccProgressDialog.h>
#include <ccCommandLineInterface.h>
#include <Neighbourhood.h>
#include <ccRasterGrid.h>

// --- for Python3 interface

struct pyccPlugins
{
static bool _isPluginFbx;
static bool isPluginFbx() { return _isPluginFbx; };
};

enum CC_DIRECTION
{
    X = 0, Y = 1, Z = 2
};

enum CC_SHIFT_MODE
{
    AUTO = 0, XYZ = 1, FIRST_GLOBAL_SHIFT = 2, NO_GLOBAL_SHIFT = 3
};
//enum GLOBAL_SHIFT_MODE
//{
//    AUTO_GLOBAL_SHIFT = 0, CUSTOM_GLOBAL_SHIFT = 1, FIRST_GLOBAL_SHIFT = 2, NO_GLOBAL_SHIFT = 3
//};

//! load a Polyline from file
/*! TODO process optional skip parameter following ccCommandLineInterface::processGlobalShiftCommand
 * \param filename
 * \param mode optional default AUTO
 * \param skip optional default 0
 * \param x optional default 0
 * \param y optional default 0
 * \param z optional default 0
 * \return polyline if success, or nullptr
 */
ccPolyline* loadPolyline(
    const char* filename,
    CC_SHIFT_MODE mode = AUTO,
    int skip = 0,
    double x = 0,
    double y = 0,
    double z = 0);

//! load any kind of entities (cloud or mesh) from a file
/*! adapted from ccCommandLineInterface::importFile
 * \param filename
 * \param mode optional default AUTO
 * \param skip optional default 0
 * \param x optional default 0
 * \param y optional default 0
 * \param z optional default 0
 * \return a vector of entities, empty if problem
 */
std::vector<ccHObject*> importFile(const char* filename,
    CC_SHIFT_MODE mode = AUTO,
    double x = 0,
    double y = 0,
    double z = 0);

//! save a point cloud to a file
/*! the file type is given by the extension
 * \param cloud
 * \param filename
 * \return IO status
 */
CC_FILE_ERROR SavePointCloud(ccPointCloud* cloud, const QString& filename);

//! save a mesh to a file
/*! the file type is given by the extension
 * \param mesh
 * \param filename
 * \return IO status
 */
CC_FILE_ERROR SaveMesh(ccMesh* mesh, const QString& filename);


//! save a vector of entities
/*! the file type is given by the extension (use .bin)
 * \param entities
 * \param filename
 * \return IO status
 */
CC_FILE_ERROR SaveEntities(std::vector<ccHObject*> entities, const QString& filename);


enum CurvatureType
{
    GAUSSIAN_CURV = 1, MEAN_CURV, NORMAL_CHANGE_RATE
};

//! Computes a geometric characteristic (see GeometricalAnalysisTools::GeomCharacteristic) on a list of clouds
/*! Computes a geometric characteristic (see GeometricalAnalysisTools::GeomCharacteristic) on a set of entities
 * \param option from (GAUSSIAN_CURV, MEAN_CURV, NORMAL_CHANGE_RATE)
 * \param list of clouds
 * \return status
 */
bool computeCurvature(CurvatureType option, double radius, std::vector<ccHObject*> clouds);

bool computeFeature(CCCoreLib::Neighbourhood::GeomFeature option, double radius, std::vector<ccHObject*> clouds);

bool computeLocalDensity(CCCoreLib::GeometricalAnalysisTools::Density option, double radius, std::vector<ccHObject*> clouds);

bool computeApproxLocalDensity(CCCoreLib::GeometricalAnalysisTools::Density option, double radius, std::vector<ccHObject*> clouds);

bool computeRoughness(double radius, std::vector<ccHObject*> clouds);

bool computeMomentOrder1(double radius, std::vector<ccHObject*> clouds);

//! Filters out points whose scalar values falls into an interval(see ccPointCloud::filterBySFValue)
/** Threshold values should be expressed relatively to the current displayed scalar field.
 \param minVal minimum value
 \param maxVal maximum value
 \return resulting cloud (remaining points)
 **/
ccPointCloud* filterBySFValue(double minVal, double maxVal, ccPointCloud* cloud);

//! Returns a default first guess for algorithms kernel size (several clouds)
/*! copied from ccLibAlgorithms::GetDefaultCloudKernelSize
 * \param list of clouds
 * \param number of nodes wanted within the radius
 * \return radius
 */
double GetPointCloudRadius(std::vector<ccHObject*> clouds, unsigned knn = 12);

//! copied from ccRegistrationTools::ICP
bool ICP(
    ccHObject* data,
    ccHObject* model,
    ccGLMatrix& transMat,
    double& finalScale,
    double& finalRMS,
    unsigned& finalPointCount,
    double minRMSDecrease,
    unsigned maxIterationCount,
    unsigned randomSamplingLimit,
    bool removeFarthestPoints,
    CCCoreLib::ICPRegistrationTools::CONVERGENCE_TYPE method,
    bool adjustScale,
    double finalOverlapRatio = 1.0,
    bool useDataSFAsWeights = false,
    bool useModelSFAsWeights = false,
    int transformationFilters = CCCoreLib::RegistrationTools::SKIP_NONE,
    int maxThreadCount = 0);

//! copied from ccEntityAction::computeNormals
bool computeNormals(std::vector<ccHObject*> selectedEntities,
    CCCoreLib::LOCAL_MODEL_TYPES model =CCCoreLib::LS,
    bool useScanGridsForComputation = true,
    PointCoordinateType defaultRadius = 0.0,
    double minGridAngle_deg =1.0,
    bool orientNormals = true,
    bool useScanGridsForOrientation = true,
    bool useSensorsForOrientation = true,
    ccNormalVectors::Orientation preferredOrientation = ccNormalVectors::UNDEFINED,
    bool orientNormalsMST = true,
    int mstNeighbors = 6,
    bool computePerVertexNormals = true);


//! Report info volume TODO: copied from Report info qCC/ccVolumeCalcTool.h
struct ReportInfoVol
{
    ReportInfoVol();

    //QString toText(int precision = 6) const;

    double volume;
    double addedVolume;
    double removedVolume;
    double surface;
    float matchingPercent;
    float ceilNonMatchingPercent;
    float groundNonMatchingPercent;
    double averageNeighborsPerCell;
};

bool ComputeVolume25D(  ReportInfoVol* reportInfo,
                        ccGenericPointCloud* ground,
                        ccGenericPointCloud* ceil,
                        unsigned char vertDim,
                        double gridStep,
                        double groundHeight,
                        double ceilHeight);

ccPointCloud* RasterizeToCloud(
	ccGenericPointCloud* cloud,
	double gridStep,
	CC_DIRECTION vertDir = CC_DIRECTION::Z,
	bool outputRasterZ = false,
	bool outputRasterSFs = false,
	bool outputRasterRGB = false,
	std::string pathToImages = ".",
	bool resample = false,
	ccRasterGrid::ProjectionType projectionType = ccRasterGrid::PROJ_AVERAGE_VALUE,
	ccRasterGrid::ProjectionType sfProjectionType = ccRasterGrid::PROJ_AVERAGE_VALUE,
	ccRasterGrid::EmptyCellFillOption emptyCellFillStrategy = ccRasterGrid::LEAVE_EMPTY,
	double customHeight = std::numeric_limits<double>::quiet_NaN(),
	ccBBox gridBBox = ccBBox());

ccMesh* RasterizeToMesh(
	ccGenericPointCloud* cloud,
	double gridStep,
	CC_DIRECTION vertDir = CC_DIRECTION::Z,
	bool outputRasterZ = false,
	bool outputRasterSFs = false,
	bool outputRasterRGB = false,
	std::string pathToImages = ".",
	bool resample = false,
	ccRasterGrid::ProjectionType projectionType = ccRasterGrid::PROJ_AVERAGE_VALUE,
	ccRasterGrid::ProjectionType sfProjectionType = ccRasterGrid::PROJ_AVERAGE_VALUE,
	ccRasterGrid::EmptyCellFillOption emptyCellFillStrategy = ccRasterGrid::LEAVE_EMPTY,
	double customHeight = std::numeric_limits<double>::quiet_NaN(),
	ccBBox gridBBox = ccBBox());

ccHObject* RasterizeGeoTiffOnly(
	ccGenericPointCloud* cloud,
	double gridStep,
	CC_DIRECTION vertDir = CC_DIRECTION::Z,
	bool outputRasterZ = false,
	bool outputRasterSFs = false,
	bool outputRasterRGB = false,
	std::string pathToImages = ".",
	bool resample = false,
	ccRasterGrid::ProjectionType projectionType = ccRasterGrid::PROJ_AVERAGE_VALUE,
	ccRasterGrid::ProjectionType sfProjectionType = ccRasterGrid::PROJ_AVERAGE_VALUE,
	ccRasterGrid::EmptyCellFillOption emptyCellFillStrategy = ccRasterGrid::LEAVE_EMPTY,
	double customHeight = std::numeric_limits<double>::quiet_NaN(),
	ccBBox gridBBox = ccBBox());

// --- internal functions (not wrapped in the Python API) ---------------------

//! initialize internal structures: should be done once, multiples calls allowed (does nothing)
struct pyCC;
pyCC* initCloudCompare();

//! copied from ccApplicationBase::setupPaths
void pyCC_setupPaths(pyCC* capi);

//! copied from ccLibAlgorithms::ComputeGeomCharacteristic
bool pyCC_ComputeGeomCharacteristic(
    CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic c,
    int subOption,
    PointCoordinateType radius,
    ccHObject::Container& entities);

//! copied from ccLibAlgorithms::GetDensitySFName
QString pyCC_GetDensitySFName(
    CCCoreLib::GeometricalAnalysisTools::Density densityType,
    bool approx,
    double densityKernelSize = 0.0);

//! copied from ccLibAlgorithms::GetDefaultCloudKernelSize
PointCoordinateType pyCC_GetDefaultCloudKernelSize(ccGenericPointCloud* cloud, unsigned knn = 12);

//! adapted from CommandRasterize::process
ccHObject* Rasterize_(
	ccGenericPointCloud* cloud,
	double gridStep,
	unsigned short vertDir = 2,            // Z direction by default
	unsigned short outputCloudOrMesh = 1,  // 0: nothing, 1: cloud, 2: mesh
	bool outputRasterZ = false,
	bool outputRasterSFs = false,
	bool outputRasterRGB = false,
	std::string pathToImages = ".",
	bool resample = false,
	ccRasterGrid::ProjectionType projectionType = ccRasterGrid::PROJ_AVERAGE_VALUE,
	ccRasterGrid::ProjectionType sfProjectionType = ccRasterGrid::PROJ_AVERAGE_VALUE,
	ccRasterGrid::EmptyCellFillOption emptyCellFillStrategy = ccRasterGrid::LEAVE_EMPTY,
	double customHeight = std::numeric_limits<double>::quiet_NaN(),
	ccBBox gridBBox = ccBBox());

//! Loaded polyline description (not in ccCommandLineInterface.h)
struct CLPolyDesc : CLEntityDesc
{
    ccPolyline* pc;

    CLPolyDesc()
        : CLEntityDesc("Unnamed poly")
        , pc( nullptr )
    {}

    CLPolyDesc(ccPolyline* poly,
                const QString& filename = QString(),
                int index = -1)
        : CLEntityDesc(filename, index)
        , pc(poly)
    {}

    CLPolyDesc(ccPolyline* poly,
                const QString& basename,
                const QString& path,
                int index = -1)
        : CLEntityDesc(basename, path, index)
        , pc(poly)
    {}

    ~CLPolyDesc() override = default;

    ccHObject* getEntity() override { return static_cast<ccHObject*>(pc); }
    const ccHObject* getEntity() const override { return static_cast<ccHObject*>(pc); }
    CL_ENTITY_TYPE getCLEntityType() const override { return CL_ENTITY_TYPE::CLOUD; }
};

#endif /* CLOUDCOMPY_PYAPI_PYCC_H_ */
