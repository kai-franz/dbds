./bad_compile.sh $1
cd ..
make clean
make
cd tests || exit
opt -load ../DbdsPass.so -dbds $1-m2r.bc -o $1_opt.bc
./cfg.sh $1_opt.bc
./cfg.sh $1-m2r.bc