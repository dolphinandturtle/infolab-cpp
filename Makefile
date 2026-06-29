coords:
	mkdir -p build
	gcc -O3 -o build/coords src/coords.c

connectivity:
	mkdir -p build
	gcc -O3 -o build/connectivity src/connectivity.c

ordering:
	mkdir -p build
	gcc -O3 -o build/ordering src/ordering.c

debug-coords:
	mkdir -p build
	gcc -o -Wall -Wextra -Werror -pedantic -Wconversion -DDEBUG= build/coords src/coords.c

debug-connectivity:
	mkdir -p build
	gcc -o -Wall -Wextra -Werror -pedantic -Wconversion -DDEBUG= build/connectivity src/connectivity.c

debug-ordering:
	mkdir -p build
	gcc -o -Wall -Wextra -Werror -pedantic -Wconversion -DDEBUG= build/ordering src/ordering.c

clean:
	rm -rf build
