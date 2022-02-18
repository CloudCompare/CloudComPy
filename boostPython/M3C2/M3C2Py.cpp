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

#include <ccPointCloud.h>
#include <ccHObject.h>
#include <ccHObjectCaster.h>
#include "pyccTrace.h"
#include "M3C2_DocStrings.hpp"

#include "qM3C2Process.h"
#include "qM3C2Dialog.h"


namespace bp = boost::python;
namespace bnp = boost::python::numpy;

ccPointCloud* computeM3C2(std::vector<ccHObject*> clouds, const QString& paramFilename)
{
    if (clouds.size() < 2)
    {
        CCTRACE("minimum two clouds required for M3C2 computation");
        return nullptr;
    }
    ccPointCloud* cloud1 = ccHObjectCaster::ToPointCloud(clouds[0]);
    ccPointCloud* cloud2 = ccHObjectCaster::ToPointCloud(clouds[1]);
    ccPointCloud* corePointsCloud = (clouds.size() > 2 ? ccHObjectCaster::ToPointCloud(clouds[2]) : nullptr);

    qM3C2Dialog dlg(cloud1, cloud2, nullptr);
    if (!dlg.loadParamsFromFile(paramFilename))
    {
        return nullptr;
    }
    dlg.setCorePointsCloud(corePointsCloud);

    QString errorMessage;
    ccPointCloud* outputCloud = nullptr; //only necessary for the command line version in fact
    if (!qM3C2Process::Compute(dlg, errorMessage, outputCloud, false))
    {
        CCTRACE(errorMessage.toStdString());
        return nullptr;
    }
    return outputCloud;
}



BOOST_PYTHON_MODULE(_M3C2)
{
    using namespace boost::python;

    scope().attr("__doc__") = M3C2_doc;

    def("computeM3C2", computeM3C2, return_value_policy<reference_existing_object>(), M3C2_computeM3C2_doc);

}
