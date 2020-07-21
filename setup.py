from pathlib import Path
from distutils.core import setup, Extension

sources = [p.as_posix() for p in Path("src/").rglob("*.cpp")]

module = Extension(
    'azul',
    sources=[
        'src/module_wrap.cxx',
    ] + sources,
    language='c++',
)

setup(
    name='azul',
    version='0.0.0',
    author="Swynfel",
    description="""Azul Environment""",
    ext_modules=[module],
    py_modules=["azul"],
)
