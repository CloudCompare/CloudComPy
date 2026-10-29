from setuptools import setup, Extension

exts = [
    Extension("cloudComPy._cloudComPy", sources=[]),
    Extension("cloudComPy._Canupo", sources=[]),
    Extension("cloudComPy._Cork", sources=[]),
    Extension("cloudComPy._CSF", sources=[]),
    Extension("cloudComPy._HoughNormals", sources=[]),
    Extension("cloudComPy._HPR", sources=[]),
    Extension("cloudComPy._M3C2", sources=[]),
    Extension("cloudComPy._MeshBoolean", sources=[]),
    Extension("cloudComPy._PCL", sources=[]),
    Extension("cloudComPy._PCV", sources=[]),
    Extension("cloudComPy._PoissonRecon", sources=[]),
    Extension("cloudComPy._RANSAC_SD", sources=[]),
    Extension("cloudComPy._SRA", sources=[]),
]

setup(ext_modules=exts)
