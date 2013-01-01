all:
	gcc -lrt -lcheck program.c -o program --coverage
	@./program
	@mkdir -p coverage
	@lcov -b . --directory . --capture --output-file coverage/coverage.info
	@genhtml -o coverage/ coverage/coverage.info

clean:
	rm program
