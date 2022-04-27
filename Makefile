all: DbdsPass.so

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) -g -O0

#INC=-I/home/user/llvm-v10/include
INC=-I/Users/kaifranz/CLionProjects/llvm-project/llvm/include

HEADS=/home/user/llvm-v10/include


%.so: %.o available-support.o
	$(CXX) -g -dylib -shared $^ -o $@

clean:
	rm -f *.o *~ *.so

.PHONY: clean all