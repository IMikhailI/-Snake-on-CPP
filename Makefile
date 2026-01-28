.PHONY: all clean install uninstall test dist dvi cli gui coverage coverage-html

COVERAGE_FLAGS = -g -O0 --coverage

all:
	cmake -S . -B build -DCMAKE_CXX_FLAGS="$(COVERAGE_FLAGS)" -DCMAKE_C_FLAGS="$(COVERAGE_FLAGS)" && $(MAKE) -C build

install:
	cmake --install build || echo 'No install target defined in CMake.'

uninstall:
	$(MAKE) -C build uninstall || echo 'Uninstall is not implemented. Remove files manually if needed.'

cli:
	build/./brickgame
	
gui:
	build/./brickgame --gui

test:
	cd build && ctest || echo 'No tests defined.'

COVERAGE_EXCLUDES = '/usr/*' '*/build/brickgame_autogen/*' '*/tests/*' '*/gui/*'

coverage-html:
	cd build && \
	lcov --capture --directory . --output-file coverage.info --ignore-errors mismatch --ignore-errors unused && \
	lcov --remove coverage.info $(COVERAGE_EXCLUDES) --output-file coverage.info --ignore-errors unused && \
	genhtml coverage.info --output-directory coverage_html && \
	xdg-open coverage_html/index.html || echo 'Open coverage_html/index.html manually.'

dist:
	@tar -czf brickgame_src.tar.gz \
		--exclude=build \
		--exclude=.git \
		--exclude='*.o' --exclude='*.so' --exclude='*.a' --exclude='*.exe' --exclude='*.out' \
		--exclude='*.tar.gz' --exclude='*.swp' --exclude='*.DS_Store' --exclude='*.pyc' --exclude='*.class' \
		--exclude='*.log' --exclude='*.tmp' --exclude='*.bak' --exclude='*.vscode' --exclude='*.idea' --exclude='*.user' \
		. 
	@echo 'Source archive brickgame_src.tar.gz created.'

dvi:
	@if [ -f Doxyfile ]; then \
		doxygen Doxyfile; \
		echo 'Documentation generated in ./html (or as specified in Doxyfile).'; \
	else \
		echo 'No Doxyfile found. Please provide a Doxyfile for documentation generation.'; \
	fi 

clang-n:
	# @echo "┏=========================================┓"
	# @echo "┃  Formatting your code for Google Style  ┃"
	# @echo "┗=========================================┛"
	cp ../materials/linters/.clang-format .
	clang-format -n **/**/*.h *.cpp **/*.cpp **/**/*.cpp **/**/*.c
	rm ./.clang-format

clang-i:
	# @echo "┏=========================================┓"
	# @echo "┃  Formatting your code for Google Style  ┃"
	# @echo "┗=========================================┛"
	cp ../materials/linters/.clang-format .
	clang-format -i **/**/*.h *.cpp **/*.cpp **/**/*.cpp **/**/*.c
	rm ./.clang-format

valgrind:
	valgrind --tool=memcheck --leak-check=yes build/./brickgame

clean:
	@if [ -d build ]; then $(MAKE) -C build clean; fi
	@rm -rf build doxygen brickgame_src.tar.gz