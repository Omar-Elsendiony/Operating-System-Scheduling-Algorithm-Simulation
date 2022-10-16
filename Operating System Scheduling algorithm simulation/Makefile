build:
	gcc -lm process_generator.c -o process_generator.out
	gcc -lm clk.c -o clk.out
	gcc -lm Trial2.c -o test
	gcc -lm Trial.c -o Trial.out
	gcc -lm scheduler.c -o scheduler.out
	gcc -lm process.c -o process.out
	gcc -lm test_generator.c -o test_generator.out
clean:
	rm -f *.out  processes.txt

all: clean build

run:
	./process_generator.out
