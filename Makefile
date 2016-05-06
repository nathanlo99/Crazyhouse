all:
	@gcc -Wall -Wextra -DDEBUG -Ofast init.c main.c -o main && ./main
