LLVM_VAR = `llvm-config --system-libs --libs all native --cxxflags --ldflags`
LLVM_VAR2 = `llvm-config --cxxflags --ldflags --system-libs --libs all`
# -I ~/llvm-project/llvm/include

compiler: parser.hpp parser.cpp tokens.cpp main.o
	rm -f compiler;\
	g++ -g build/*.o -lfl -o compiler $(LLVM_VAR2) -pthread -ltinfo -std=c++14 -frtti;


main.o: nodes.o
	g++ -g -c main.cpp $(LLVM_VAR2) -frtti -o ./build/main.o -std=c++14

nodes.o:
	g++ -g -c ./nodes/unity.cpp $(LLVM_VAR2) -frtti -o ./build/nodes.o -std=c++14

visitor.o:
	g++ -g -c ./visitor/visitor.cpp $(LLVM_VAR2) -o ./build/visitor.o -std=c++14

tests: compiler
	g++ -g tests_driver.cpp -o testgcc

parser.cpp parser.hpp: parser.y
	bison -d -o parser.cpp parser.y --report=all

tokens.cpp: tokens.l parser.hpp
	flex -o tokens.cpp tokens.l

clean:
	rm -f testgcc tokens.cpp parser.cpp parser.hpp compiler node.h.gch parser.output nodeTest src.h.gch ./build/*.o
	