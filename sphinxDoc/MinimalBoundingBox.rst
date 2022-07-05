
================================
minimalBoundingBox Python plugin
================================

``minmimalBoundingBox`` is a pure Python plugin built with CloudComPy.

This tool is provided as an example of CloudComPy pure Python extension.

The ``findRotation`` function finds a minimal bounding box, not oriented along the main axes,
and the corresponding rotation.
It is not proven that the solution meets to the minimum, and the performance and robustness
can certainly be improved.

.. autofunction:: cloudComPy.minimalBoundingBox.dist2
 
.. autofunction:: cloudComPy.minimalBoundingBox.findRotation
 
.. autofunction:: cloudComPy.minimalBoundingBox.isocahedron
 
.. autofunction:: cloudComPy.minimalBoundingBox.section
 
.. autofunction:: cloudComPy.minimalBoundingBox.zinv
