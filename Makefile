domgen:
	mkdir -p build
	gcc -O3 -o build/domgen src/domgen.c src/string.c

ordering:
	mkdir -p build
	gcc -O3 -o build/ordering src/ordering.c

debug-domgen:
	mkdir -p build
	gcc -Wall -Wextra -Werror -pedantic -Wconversion -DDEBUG= -o build/domgen src/domgen.c src/string.c

debug-ordering:
	mkdir -p build
	gcc -Wall -Wextra -Werror -pedantic -Wconversion -DDEBUG= -o build/ordering src/ordering.c

clean:
	rm -rf build
