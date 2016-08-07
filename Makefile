WARNINGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wformat=2\
 -Winit-self -Wmissing-declarations -Wredundant-decls -Wshadow\
 -Wstrict-overflow=5 -Wswitch-default -Wundef

all:
	llvm-gcc $(WARNINGS) -std=c11 -Ofast *.c -o main
	./main

debug:
	llvm-gcc $(WARNINGS) -std=c11 -DDEBUG *.c -o main
