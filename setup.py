from pathlib import Path
from setuptools import setup, Extension, find_packages
import pybind11

__version__ = '0.2.1'

sources = ["src/py_main.cpp"] + \
    sorted(p.as_posix() for dir in ["game", "players", "rules", "state"]
           for p in Path(f"src/{dir}/").rglob("*.cpp"))

module = Extension(
    'ceramic',
    sources=sources,
    include_dirs=[
        "src",
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
