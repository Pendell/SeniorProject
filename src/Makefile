LLVM_VAR = `llvm-config --libs core native --cxxflags --ldflags`


compiler: parser.hpp parser.cpp tokens.cpp main.o
	g++ ./build/main.o -lfl -o compiler -I ./build/*.o

main.o: nodes.o visitor.o
	g++ -c main.cpp $(LLVM_VAR) -frtti -o ./build/main.o

nodes.o:
	g++ -c ./nodes/node.cpp $(LLVM_VAR) -frtti -o ./build/nodes.o

visitor.o:
	g++ -c ./visitor/visitor.cpp $(LLVM_VAR) -o ./build/visitor.o

test:
	g++ node.h tests/nodeComparisonTest.cpp -o nodeTest

parser.cpp parser.hpp: parser.y
	bison -d -o parser.cpp parser.y --report=all

tokens.cpp: tokens.l parser.hpp
	flex -o tokens.cpp tokens.l

clean:
	rm -f tokens.cpp parser.cpp parser.hpp compiler node.h.gch parser.output nodeTest src.h.gch ./build/*.o
