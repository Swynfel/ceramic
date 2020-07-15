mkdir -p build target
rm -r target/*
swig -c++ -python -outdir target src/module.i
python setup.py build_ext --inplace
rm src/module_wrap.cxx
mv _azul.*.so target/_azul.so
