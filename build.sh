swig -c++ -python -outdir . src/module.i
python setup.py build_ext --inplace
rm src/module_wrap.cxx
mkdir -p target
mv _azul.*.so target/_azul.so
mv azul.py target/azul.py
