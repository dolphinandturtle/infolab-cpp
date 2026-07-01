domgen:
	mkdir -p build
	gcc -O3 -o build/domgen src/domgen.c src/mesh.c src/graph.c src/string.c

ordering:
	mkdir -p build
	gcc -O3 -o build/ordering src/ordering.c

sparse:
	mkdir -p build
	gcc -O3 -o build/sparce -lm src/sparse.c

debug-domgen:
	mkdir -p build
	gcc -Wall -Wextra -Werror -pedantic -Wconversion -DDEBUG= -o build/domgen src/domgen.c src/mesh.c src/graph.c src/string.c

debug-ordering:
	mkdir -p build
	gcc -Wall -Wextra -Werror -pedantic -Wconversion -DDEBUG= -o build/ordering src/ordering.c

debug-sparse:
	mkdir -p build
	gcc -Wall -Wextra -Werror -pedantic -Wconversion -DDEBUG= -o build/sparse -lm src/sparse.c

test:
	mkdir -p build
	gcc -Wall -Wextra -Werror -pedantic -Wconversion -DDEBUG= -o build/test src/test.c src/string.c

clean:
	rm -rf build
