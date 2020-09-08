from pathlib import Path
from setuptools import setup, Extension, find_packages
import pybind11

__version__ = '0.0.4'

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
    packages=find_packages(),
    test_suite='tests',
    zip_safe=False,
)
