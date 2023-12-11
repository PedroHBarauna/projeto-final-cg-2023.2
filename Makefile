all:
	gcc -o camera humanoide.c -lglut -lGL -lGLU -lm
	./camera