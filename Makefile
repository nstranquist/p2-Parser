# Makefile
# TODO: Add "-82" or whatever Nathen said

parser: main.o parser.o printTree.o 
	g++ main.o parser.o printTree.o -o parser

main.o: main.cpp printTree.h parser.h token.h
	g++ -c -Wall main.cpp

parser.o: parser.cpp parser.h token.h table.h
	g++ -c -Wall parser.cpp

printTree.o: printTree.cpp printTree.h parser.h
	g++ -c -Wall printTree.cpp