mkdir -p build target
rm -r target/*
swig -c++ -python -outdir build -builtin -interface azul src/module.i
python setup.py build_ext --inplace
rm src/module_wrap.cxx
rm build/azul.py
mv azul.*.so target/azul.so
