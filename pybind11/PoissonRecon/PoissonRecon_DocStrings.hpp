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

#ifndef PoissonRecon_DOCSTRINGS_HPP_
#define PoissonRecon_DOCSTRINGS_HPP_


const char* PoissonRecon_doc= R"(
PoissonRecon is a standard plugin of cloudComPy.

The availability of the plugin can be tested with the isPluginPoissonRecon function:

  isPoissonRecon_available = cc.isPluginPoissonRecon()

PoissonRecon is a submodule of cloudCompy:
::

  import cloudComPy as cc
  # ...
  if cc.isPluginPoissonRecon():
      import cloudComPy.PoissonRecon
      mesh = cc.PoissonRecon.PR.PoissonReconstruction(...)
)";

const char* PoissonRecon_PR_doc = R"(
We only use the static method PoissonReconstruction of this class.
)";

const char* PoissonRecon_PoissonReconstruction_doc = R"(
Build a mesh on a point cloud using the PoissonReconstruction algorithm.
    Wrapper to the 'Poisson Surface Reconstruction (Version 9)' algorithm 
    'Poisson Surface Reconstruction', M. Kazhdan, M. Bolitho, and H. Hoppe
    Symposium on Geometry Processing (June 2006), pages 61--70
    http://www.cs.jhu.edu/~misha/Code/PoissonRecon/

:param ccPointCloud pc: the point cloud
:param int,optional depth: octree depth, default 8
                           The maximum depth of the tree that will be used for surface reconstruction.
                           Running at depth d corresponds to solving on a 2^d x 2^d x 2^d.
                           Note that since the reconstructor adapts the octree to the sampling density,
                           the specified reconstruction depth is only an upper bound.
:param float,optional finestCellWidth: default 0.0
                                       The target width of the finest level octree cells (ignored if depth is specified)
:param float,optional samplesPerNode: default 1.5
                                      The minimum number of sample points that should fall within an octree node
                                      as the octree construction is adapted to sampling density.
                                      This parameter specifies the minimum number of points that should fall within an octree node.
                                      For noise-free samples, small values in the range [1.0 - 5.0] can be used. 
                                      For more noisy samples, larger values in the range [15.0 - 20.0]
                                      may be needed to provide a smoother, noise-reduced, reconstruction.
:param bool,optional withColors: default False
                                 This flag tells the reconstructor to read in color values with the input points
                                 and extrapolate those to the vertices of the output.
:param bool,optional density: default False
                              If this flag is enabled, the sampling density is written out with the vertices
:param float,optional pointWeight: default 2.0
                                   The importance that interpolation of the point samples is given in the formulation
                                   of the screened Poisson equation. The results of the original (unscreened)
                                   Poisson Reconstruction can be obtained by setting this value to 0.
:param int,optional threads: default 1
                             This parameter specifies the number of threads across which the solver should be parallelized
:param bool,optional linearFit: default False
                                Enabling this flag has the reconstructor use linear interpolation
                                to estimate the positions of iso-vertices.
:param PoissonReconLib::Parameters::BoundaryType,optional boundary: default NEUMANN
                                                                    Boundary type for the finite elements
 
:return: the mesh (or None if failed)
:rtype: ccMesh
)";

const char* PoissonRecon_initTrace_PoissonRecon_doc=R"(
Debug trace must be initialized for each Python module.

Done in module init, following the value of environment variable _CCTRACE_ ("ON" if debug traces wanted)
)";


#endif /* PoissonRecon_DOCSTRINGS_HPP_ */
