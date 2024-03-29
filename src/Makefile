all: main

OBJS = grammar.o       \
		token.o        \
		main.o         \
		context.o      \
		typesystem.o   \
		AST.o          \

LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++11 -g
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic -L/usr/local/lib 
LIBS = `$(LLVMCONFIG) --libs`

clean:
	$(RM) -rf grammar.cpp grammar.hpp token.cpp main out test out.ll test.ll $(OBJS)

main.cpp: AST.hpp context.h

context.cpp: context.h grammar.hpp

typesystem.cpp: typesystem.h

AST.cpp: global.h AST.hpp context.h

grammar.cpp: grammar.y
	bison -d -o $@ $<

grammar.hpp: grammar.cpp

token.cpp: token.l grammar.hpp
	flex -o $@ $<

%.o: %.cpp
	g++ -c $(CPPFLAGS) -o $@ $<

main: $(OBJS)
	g++ $(CPPFLAGS) -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

run: main test.c
	./main < test.c
	clang out.ll -o out
	./out

compare: test.c
	clang -S -emit-llvm test.c -o test.ll

target_calc: main ../target/calc.c
	./main < ../target/calc.c
	clang out.ll -o out
	./out

target_kmp: main ../target/kmp.c
	./main < ../target/kmp.c
	clang out.ll -o out
	./out

target_p: main ../target/palindrome.c
	./main < ../target/palindrome.c
	clang out.ll -o out
	./out
