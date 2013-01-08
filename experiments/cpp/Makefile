all:
	g++ -lrt -lcheck program.cpp o_tree.cpp tree_tests.cpp -o program --coverage
	@./program
	@mkdir -p coverage
	@lcov -b . --directory . --capture --output-file coverage/coverage.info
	@genhtml -o coverage/ coverage/coverage.info

clean:
	rm program *.gcda *.gcno 
