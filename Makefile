all: DbdsPass.so

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) -g -O0

INC=-I/home/user/llvm-v10/include

HEADS=/home/user/llvm-v10/include


%.so: %.o
	$(CXX) -dylib -shared $^ -o $@

clean:
	rm -f *.o *~ *.so

.PHONY: clean all