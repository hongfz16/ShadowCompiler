all: main

OBJS = grammar.o       \
		token.o        \
		main.o         \
		codeGen.o      \
		otherContext.o \
		TypeSystem.o   \

LLVMCONFIG = llvm-config-7
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++11
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic -L/usr/local/lib -ljsoncpp
LIBS = `$(LLVMCONFIG) --libs`

clean:
	$(RM) -rf grammar.cpp grammar.hpp token.cpp main $(OBJS)

main.cpp: AST.hpp otherContext.h TypeSystem.h#context.hpp

codeGen.cpp: AST.hpp otherContext.h TypeSystem.h#context.hpp

TypeSystem.cpp: TypeSystem.h

otherContext.cpp: otherContext.h

grammar.cpp: grammar.y
	bison -d -o $@ $<

grammar.hpp: grammar.cpp

token.cpp: token.l grammar.hpp
	flex -o $@ $<

%.o: %.cpp
	g++ -c $(CPPFLAGS) -o $@ $<

main: $(OBJS)
	g++ $(CPPFLAGS) -o $@ $(OBJS) $(LIBS) $(LDFLAGS)