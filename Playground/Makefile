all: main

LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++11
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic -L/usr/local/lib -ljsoncpp
LIBS = `$(LLVMCONFIG) --libs`

main: main.cpp
	g++ main.cpp $(CPPFLAGS) $(LIBS) $(LDFLAGS) -o main

run: main
	./main
	clang out.ll -o out
	./out

clean:
	rm main out out.ll