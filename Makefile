
all:
	g++ -std=c++11 -Wall RedBlackTree.cpp RedBlackTreeTests.cpp -o rbt-tests

run:
	./rbt-tests

try: all run