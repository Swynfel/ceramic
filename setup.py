from pathlib import Path
from distutils.core import setup, Extension
import pybind11

__version__ = '0.0.2'

sources = sorted(p.as_posix() for p in Path("src/").rglob("*.cpp"))

module = Extension(
    'ceramic',
    sources=sources,
    include_dirs=[
        pybind11.get_include()
    ],
    language='c++',
)

setup(
    name='ceramic',
    version=__version__,
    author="Swynfel",
    description="""Azul-like Game Environment""",
    ext_modules=[module],
    py_modules=["ceramic"],
)
