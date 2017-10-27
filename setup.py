from distutils.core import setup
from Cython.Build import cythonize

setup(
        name = 'Scoreline',
        ext_modules = cythonize("simulate_progression.pyx")
        )

