cd ..
make clean
make
cd tests || exit
opt -load ../DbdsPass.so -dbds test1-m2r.bc -o test1_opt.bc