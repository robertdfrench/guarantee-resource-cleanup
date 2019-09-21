test: a.out
	./a.out

a.out: main.o pipe.o
	$(CC) $^ -o $@

main.o: main.c pipe.h
	$(CC) $< -c -o $@

pipe.o: pipe.c pipe.h
	$(CC) $< -c -o $@

clean:
	rm -rf *.out *.o
