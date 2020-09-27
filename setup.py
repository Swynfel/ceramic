from pathlib import Path
from setuptools import setup, Extension, find_packages

with open('VERSION', 'r') as version:
    __version__ = version.readline()

sources = ["src/py_main.cpp"] + \
    sorted(p.as_posix() for dir in ["game", "players", "rules", "state"]
           for p in Path(f"src/{dir}/").rglob("*.cpp"))


class pybind11_get_include:
    # pylint: disable=too-few-public-methods
    def __str__(self):
        # pylint: disable=import-outside-toplevel
        import pybind11
        return pybind11.get_include()


module = Extension(
    'ceramic',
    sources=sources,
    include_dirs=[
        "src",
        pybind11_get_include()
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
    setup_requires=["pybind11"],
    test_suite='tests',
    zip_safe=False,
)
