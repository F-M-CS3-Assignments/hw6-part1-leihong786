all:
	g++ -std=c++11 -Wall -g RedBlackTree.cpp RedBlackTreeTestsFirstStep.cpp -o rbt-tests-first
	g++ -std=c++11 -Wall -g RedBlackTree.cpp RedBlackTreeTests.cpp -o rbt-tests

run:
	./rbt-tests-first
	./rbt-tests

try: all run
