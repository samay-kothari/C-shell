all:
	gcc -std=c11 -g -w *.c -o shell
	touch .shell_history.txt