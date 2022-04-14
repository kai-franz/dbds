cd ..
make clean
make
cd tests || exit
opt -load ../DbdsPass.so -dbds figure1a-m2r.bc -o figure1a_opt.bc