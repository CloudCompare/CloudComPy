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

#include "cloudComPy.hpp"

#include <QString>
#include <vector>
#include <QtCore>
#include <QtConcurrentRun>
#include <QApplication>

#include <ccPointCloud.h>
#include <ccScalarField.h>
#include <ccMesh.h>

#include <qPoissonRecon.h>
#include <PoissonReconLib.h>

#include "pyccTrace.h"
#include "PoissonRecon_DocStrings.hpp"

void initTrace_PoissonRecon()
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
}

// from qPoissonRecon.cpp
template <typename Real>
class PointCloudWrapper : public PoissonReconLib::ICloud<Real>
{
public:
    explicit PointCloudWrapper(const ccPointCloud& cloud) : m_cloud(cloud) {}

    virtual size_t size() const { return m_cloud.size(); }
    virtual bool hasNormals() const { return m_cloud.hasNormals(); }
    virtual bool hasColors() const { return m_cloud.hasColors(); }
    virtual void getPoint(size_t index, Real* coords) const
    {
        if (index >= m_cloud.size())
        {
            assert(false);
            return;
        }
        //point
        const CCVector3* P = m_cloud.getPoint(static_cast<unsigned>(index));
        coords[0] = static_cast<Real>(P->x);
        coords[1] = static_cast<Real>(P->y);
        coords[2] = static_cast<Real>(P->z);
    }

    virtual void getNormal(size_t index, Real* coords) const
    {
        if (index >= m_cloud.size() || !m_cloud.hasNormals())
        {
            assert(false);
            return;
        }

        const CCVector3& N = m_cloud.getPointNormal(static_cast<unsigned>(index));
        coords[0] = static_cast<Real>(N.x);
        coords[1] = static_cast<Real>(N.y);
        coords[2] = static_cast<Real>(N.z);
    }

    virtual void getColor(size_t index, Real* rgb) const
    {
        if (index >= m_cloud.size() || !m_cloud.hasColors())
        {
            assert(false);
            return;
        }

        const ccColor::Rgb& color = m_cloud.getPointColor(static_cast<unsigned>(index));
        rgb[0] = static_cast<Real>(color.r);
        rgb[1] = static_cast<Real>(color.g);
        rgb[2] = static_cast<Real>(color.b);
    }

protected:
    const ccPointCloud& m_cloud;
};

template <typename Real>
class MeshWrapper : public PoissonReconLib::IMesh<Real>
{
public:
    explicit MeshWrapper(ccMesh& mesh, ccPointCloud& vertices, CCCoreLib::ScalarField* densitySF = nullptr)
        : m_mesh(mesh)
        , m_vertices(vertices)
        , m_densitySF(densitySF)
        , m_error(false)
    {}

    bool checkMeshCapacity()
    {
        if (m_error)
        {
            //no need to go further
            return false;
        }
        if (m_mesh.size() == m_mesh.capacity() && !m_mesh.reserve(m_mesh.size() + 1024))
        {
            m_error = true;
            return false;
        }
        return true;
    }

    bool checkVertexCapacity()
    {
        if (m_error)
        {
            //no need to go further
            return false;
        }
        if (m_vertices.size() == m_vertices.capacity() && !m_vertices.reserve(m_vertices.size() + 4096))
        {
            m_error = true;
            return false;
        }
        return true;
    }

    virtual void addVertex(const Real* coords) override
    {
        if (!checkVertexCapacity())
        {
            return;
        }
        CCVector3 P = CCVector3::fromArray(coords);
        m_vertices.addPoint(P);
    }

    virtual void addNormal(const Real* coords) override
    {
        if (!checkVertexCapacity())
        {
            return;
        }
        if (!m_vertices.hasNormals() && !m_vertices.reserveTheNormsTable())
        {
            m_error = true;
            return;
        }
        CCVector3 N = CCVector3::fromArray(coords);
        m_vertices.addNorm(N);
    }

    virtual void addColor(const Real* rgb) override
    {
        if (!checkVertexCapacity())
        {
            return;
        }
        if (!m_vertices.hasColors())
        {
            if (!m_vertices.reserveTheRGBTable())
            {
                m_error = true;
                return;
            }
        }
        m_vertices.addColor(static_cast<ColorCompType>(std::min((Real)255, std::max((Real)0, rgb[0]))),
            static_cast<ColorCompType>(std::min((Real)255, std::max((Real)0, rgb[1]))),
            static_cast<ColorCompType>(std::min((Real)255, std::max((Real)0, rgb[2]))));
    }

    virtual void addDensity(double d) override
    {
        if (!m_densitySF)
        {
            return;
        }
        if (m_densitySF->size() == m_densitySF->capacity() && !m_densitySF->reserveSafe(m_densitySF->size() + 4096))
        {
            m_error = true;
            return;
        }
        m_densitySF->addElement(static_cast<ScalarType>(d));
    }

    void addTriangle(size_t i1, size_t i2, size_t i3) override
    {
        if (!checkMeshCapacity())
        {
            return;
        }
        m_mesh.addTriangle(static_cast<unsigned>(i1), static_cast<unsigned>(i2), static_cast<unsigned>(i3));
    }

    bool isInErrorState() const { return m_error; }

protected:
    ccMesh& m_mesh;
    ccPointCloud& m_vertices;
    bool m_error;
    CCCoreLib::ScalarField* m_densitySF;
};

static PoissonReconLib::Parameters s_params;
static ccPointCloud* s_cloud = nullptr;
static ccMesh* s_mesh = nullptr;
static ccPointCloud* s_meshVertices = nullptr;
static CCCoreLib::ScalarField* s_densitySF = nullptr;

static bool doReconstruct()
{
    //invalid parameters
    if (!s_cloud || !s_mesh || !s_meshVertices)
    {
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    MeshWrapper<PointCoordinateType> meshWrapper(*s_mesh, *s_meshVertices, s_densitySF);
    PointCloudWrapper<PointCoordinateType> cloudWrapper(*s_cloud);

    if (!PoissonReconLib::Reconstruct(s_params, cloudWrapper, meshWrapper) || meshWrapper.isInErrorState())
    {
        return false;
    }

    qint64 elpased_msec = timer.elapsed();
    ccLog::Print(QString("[PoissonRecon] Duration: %1 s").arg(elpased_msec / 1000.0, 0, 'f', 1));

    return true;
}

struct PoissonRecon
{
        // from qPoissonRecon::doAction()
    static ccMesh* PoissonReconstruction(   ccPointCloud* pc,
                                            int depth=8,
                                            double finestCellWidth = 0.0,
                                            double samplesPerNode = 1.5,
                                            bool withColors = false,
                                            bool density = false,
                                            double pointWeight=2.0,
                                            int threads = 1,
                                            bool linearFit = false,
                                            PoissonReconLib::Parameters::BoundaryType boundary = PoissonReconLib::Parameters::BoundaryType::NEUMANN)
    {
        if (!pc->hasNormals())
        {
            CCTRACE("Cloud must have normals!");
            return nullptr;
        }

        static unsigned s_lastEntityID = 0;
        static double s_defaultResolution = 0.0;
        static bool s_depthMode = true;
        if (s_defaultResolution == 0.0 || s_lastEntityID != pc->getUniqueID())
        {
            s_defaultResolution = pc->getOwnBB().getDiagNormd() / 200.0;
            s_lastEntityID = pc->getUniqueID();
        }

        bool cloudHasColors = pc->hasColors();

        s_params.depth = depth;
        s_params.finestCellWidth = finestCellWidth;
        s_params.samplesPerNode = samplesPerNode;
        s_params.withColors = withColors;
        s_params.density = density;
        s_params.pointWeight = pointWeight;
        s_params.threads = threads;
        s_params.linearFit = linearFit;
        s_params.boundary = boundary;

        /*** RECONSTRUCTION PROCESS ***/

        ccScalarField* densitySF = nullptr;
        ccPointCloud* newPC = new ccPointCloud("vertices");
        ccMesh* newMesh = new ccMesh(newPC);
        newMesh->addChild(newPC);

        //run in a separate thread
        bool result = false;
        {
            //start message
            CCTRACE("[PoissonRecon] Job started (level " << s_params.depth << " " << s_params.threads <<  "threads)");

            //run in a separate thread
            s_cloud = pc;
            s_mesh = newMesh;
            s_meshVertices = newPC;

            if (s_params.density)
            {
                s_densitySF = (densitySF = new ccScalarField("Density"));
            }

            QFuture<bool> future = QtConcurrent::run(doReconstruct);

            //wait until process is finished!
            while (!future.isFinished())
            {
    #if defined(CC_WINDOWS)
                ::Sleep(500);
    #else
                usleep(500 * 1000);
    #endif

                QApplication::processEvents();
            }

            result = future.result();

            s_cloud = nullptr;
            s_mesh = nullptr;
            s_meshVertices = nullptr;

            QApplication::processEvents();
        }

        if (!result)
        {
            if (densitySF)
            {
                densitySF->release();
                densitySF = nullptr;
            }
            delete newMesh;
            newMesh = nullptr;
            CCTRACE("Reconstruction failed!");
            return nullptr;
        }

        //success message
        CCTRACE("[PoissonRecon] Job finished (" << newMesh->size() << " triangles, " << newPC->size() << " vertices)");

        newMesh->setName(QString("Mesh[%1] (level %2)").arg(pc->getName()).arg(s_params.depth));
        newPC->setEnabled(false);
        newMesh->setVisible(true);
        newMesh->computeNormals(true);
        if (!cloudHasColors)
        {
            newPC->unallocateColors();
            newPC->showColors(false);
        }
        newMesh->showColors(newPC->hasColors());

        if (densitySF)
        {
            densitySF->computeMinAndMax();
            densitySF->showNaNValuesInGrey(false);
            int sfIdx = newPC->addScalarField(densitySF);
            newPC->setCurrentDisplayedScalarField(sfIdx);
            newPC->showSF(true);
            newMesh->showColors(newPC->colorsShown());
            newMesh->showSF(true);
        }

        //copy Global Shift & Scale information
        newPC->copyGlobalShiftAndScale(*pc);

        return newMesh;
    }
};


PYBIND11_MODULE(_PoissonRecon, m10)
{
    m10.doc() = PoissonRecon_doc;

    py::enum_<PoissonReconLib::Parameters::BoundaryType>(m10, "BoundaryType")
        .value("FREE", PoissonReconLib::Parameters::BoundaryType::FREE)
        .value("DIRICHLET", PoissonReconLib::Parameters::BoundaryType::DIRICHLET)
        .value("NEUMANN", PoissonReconLib::Parameters::BoundaryType::NEUMANN)
        .export_values();

    py::class_<PoissonRecon>(m10, "PR", PoissonRecon_PR_doc)
        .def_static("PoissonReconstruction", &PoissonRecon::PoissonReconstruction,
            ::py::arg("pc"),
            ::py::arg("depth")=8,
            ::py::arg("finestCellWidth")=0.0,
            ::py::arg("samplesPerNode")=1.5,
            ::py::arg("withColors")=false,
            ::py::arg("density")=false,
            ::py::arg("pointWeight")=2.0,
            ::py::arg("threads")=1,
            ::py::arg("linearFit")=false,
            ::py::arg("boundary")=PoissonReconLib::Parameters::BoundaryType::NEUMANN,
            PoissonRecon_PoissonReconstruction_doc);
        ;

   m10.def("initTrace_PoissonRecon", &initTrace_PoissonRecon, PoissonRecon_initTrace_PoissonRecon_doc);
}

