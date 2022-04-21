cd ..
make clean
make
cd tests || exit
opt -load ../DbdsPass.so -dbds srtest-m2r.bc -o srtest_opt.bc