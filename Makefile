CC = gcc
CC += -m32 -no-pie -fno-pie

LDLIBS = -lm

CFLAGS ?= -O2 -g
CFLAGS += -std=gnu99
CFLAGS += -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
	-Wswitch-default -Wpointer-arith -Wtype-limits -Wempty-body \
	-Wstrict-prototypes -Wold-style-declaration -Wold-style-definition \
	-Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs \
	-Wstack-usage=4096 -Wmissing-prototypes -Wfloat-equal -Wabsolute-value

CFLAGS += -fsanitize=undefined -fsanitize-undefined-trap-on-error


.DELETE_ON_ERROR:

.PHONY: all
all: integral

integral: integral.c integral.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

integral.o: func.asm
	nasm -f elf32 -o $@ $<


.PHONY: clean
clean:
	rm -f *.o integral


.PHONY: test
test: integral
	./integral --test-root 4:6:0.1:2:0.0001:0.854
	./integral --test-root 5:6:2.5:3.5:0.0001:3.244
	./integral --test-root 4:5:3.5:4.5:0.0001:3.848
	./integral --test-root 1:3:2.02:2.3:0.0001:2.192
	./integral --test-root 2:3:3.5:5.5:0.0001:4.225
	./integral --test-root 1:2:5.5:6.5:0.0001:6.096
	@echo
	./integral --test-integral 4:0:3:0.00001:11.70000
	./integral --test-integral 5:-1:4:0.00001:-21.25
	./integral --test-integral 6:0.1:2.7:0.00001:9.88751
