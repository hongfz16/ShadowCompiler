all: main

OBJS = grammar.o       \
		token.o        \
		main.o         \
		context.o      \
		typesystem.o   \
		AST.o          \

LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++11
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic -L/usr/local/lib -ljsoncpp
LIBS = `$(LLVMCONFIG) --libs`

clean:
	$(RM) -rf grammar.cpp grammar.hpp token.cpp main $(OBJS)

main.cpp: AST.hpp context.h

context.cpp: context.h

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

run: main
	./main < test.code