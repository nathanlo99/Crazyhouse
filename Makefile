WARNINGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wformat=2\
 -Winit-self -Wmissing-declarations -Wredundant-decls -Wshadow\
 -Wstrict-overflow=5 -Wswitch-default -Wundef

FLAGS = $(WARNINGS) -std=c11

all:
	llvm-gcc $(FLAGS) -Ofast *.c -o main
	./main

debug:
	llvm-gcc $(FLAGS) -DDEBUG *.c -o main
	./main
