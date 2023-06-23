#!/bin/bash

gcc -c -ggdb -fno-builtin -nostdlib -fno-stack-protector ./src/entry.c ./src/malloc.c ./src/stdio.c ./src/string.c ./src/printf.c

ar -rs rcrt.a malloc.o printf.o stdio.o string.o

gcc -c -ggdb -fno-builtin -nostdlib -fno-stack-protector ./tests/test.c

mv *.o *.a ./lib

ld -static -e rcrt_entry ./lib/entry.o ./lib/test.o ./lib/rcrt.a -o ./bin/test

ls -l ./bin/test


