test: a.o b.o c.o
	# $^ : All dependent files.
	gcc -o test $^		

%.o	: %.c
	# $@ : Object file.
	# $< : The first dependent file.
	gcc -c -o $@ $<		

clean:
	rm *.o test

.PHONY: clean