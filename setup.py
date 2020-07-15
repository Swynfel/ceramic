from distutils.core import setup, Extension

module = Extension(
    '_azul',
    sources=[
        'src/module_wrap.cxx',
        'src/tiles.cpp'
    ],
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
