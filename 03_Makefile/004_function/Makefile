
# a. $(foreach var, list, text)
A = a b c
B = $(foreach f, $(A), $(f).o)

# b. $(filter pattern..., text)
#	 $(filter-out pattern..., text)
C = a b c d/
D = $(filter %/, $(C))
E = $(filter-out %/, $(C))

# c. $(wildcard pattern)
files = $(wildcard *.c)
files2 = a.c b.c c.c d.c e.c
files3 = $(wildcard $(files2))

# d. $(patsubst pattern, replacement, $(var))
dep_files = $(patsubst %.c, %.o, $(files2))

all:
	@echo B = $(B)
	@echo D = $(D)
	@echo E = $(E)
	@echo files = $(files)
	@echo files3 = $(files3)
	@echo dep_files = $(dep_files) 