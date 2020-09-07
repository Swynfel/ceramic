mkdir -p build target
rm -r target/*
swig -c++ -python -outdir build -builtin -interface ceramic src/module.i
python setup.py build_ext --inplace
rm src/module_wrap.cxx
rm build/ceramic.py
mv ceramic.*.so target/ceramic.so
