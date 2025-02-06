
objs = a.o b.o c.o
dep_files := $(patsubst %, .%.d, $(objs))
dep_files := $(wildcard $(dep_files))

CFLAGS = -Werror -Iinclude

# $^ : All dependent files.
test: $(objs)
	gcc -o test $^		

ifneq ($(dep_files), )
include $(dep_files)
endif

# $@ : Object file.
# $< : The first dependent file.
%.o	: %.c
	gcc $(CFLAGS) -c -o $@ $<	-MD -MF .$@.d

clean:
	rm *.o test

distclean:
	rm $(dep_files)

.PHONY: clean