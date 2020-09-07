from pathlib import Path
from distutils.core import setup, Extension

sources = [p.as_posix() for p in Path("src/").rglob("*.cpp")]

module = Extension(
    'ceramic',
    sources=[
        'src/module_wrap.cxx',
    ] + sources,
    language='c++',
)

setup(
    name='ceramic',
    version='0.0.1',
    author="Swynfel",
    description="""Azul-like Game Environment""",
    ext_modules=[module],
    py_modules=["ceramic"],
)
