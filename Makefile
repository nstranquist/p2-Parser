# Makefile

parser: main.o parser.o scanner.o printTree.o tree.o
	g++ main.o parser.o scanner.o printTree.o tree.o -o parser

main.o: main.cpp printTree.h parser.h token.h tree.h node.h
	g++ -c -Wall -CC -std=c++11 main.cpp

parser.o: parser.cpp parser.h token.h table.h tree.h node.h
	g++ -c -Wall -CC -std=c++11 parser.cpp

scanner.o: scanner.cpp scanner.h token.h table.h
	g++ -c -Wall -CC -std=c++11 scanner.cpp

printTree.o: printTree.cpp printTree.h node.h tree.h
	g++ -c -Wall -CC -std=c++11 printTree.cpp

tree.o: tree.cpp tree.h
	g++ -c -Wall -CC -std=c++11 tree.cpp