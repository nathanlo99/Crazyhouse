WARNINGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual\
-Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op\
-Wmissing-declarations -Wredundant-decls -Wshadow\
-Wstrict-overflow=5 -Wswitch-default -Wundef

all:
	gcc $(WARNINGS) -DDEBUG -Ofast *.c -o main
	./main
