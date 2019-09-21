test: a.out
	./a.out

a.out: main.o pipe.o fork.o
	$(CC) $^ -o $@

main.o: main.c pipe.h fork.h
	$(CC) $< -c -o $@

pipe.o: pipe.c pipe.h
	$(CC) $< -c -o $@

fork.o: fork.c fork.h
	$(CC) $< -c -o $@

clean:
	rm -rf *.out *.o
