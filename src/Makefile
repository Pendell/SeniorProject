parser.cpp parser.hpp: parser.y
	bison -d -o parser.cpp parser.y --report=all

tokens.cpp: tokens.l parser.hpp
	flex -o tokens.cpp tokens.l

compiler: parser.hpp parser.cpp tokens.cpp
	g++ node.h
	g++ parser.cpp tokens.cpp -lfl -o compiler -I ~/llvm-project/llvm/include/  -I ~/llvm-project/build/include

clean:
	rm tokens.cpp parser.cpp parser.hpp compiler node.h.gch parser.output
