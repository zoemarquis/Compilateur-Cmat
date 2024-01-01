
CC=gcc
CFLAGS=-g -fcommon

all: cmat

cmat: cmat.c EXPR.tab.o lex.yy.o symbtab.o generation.o
	${CC} -o $@ $^

EXPR.tab.o: EXPR.tab.c symbtab.h generation.h

EXPR.tab.c: EXPR.y
	bison -d $<

lex.yy.o: lex.yy.c

lex.yy.c: EXPR.lex EXPR.tab.c
	flex EXPR.lex

symbtab.o: CFLAGS+=-Wall -Wextra
symbtab.o: symbtab.c symbtab.h
	${CC} ${CFLAGS} -c $< -o $@

generation.o: CFLAGS+=-Wall -Wextra 
generation.o: generation.c generation.h symbtab.h 
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f cmat *.o EXPR.tab.c EXPR.tab.h lex.yy.c *~
	rm -rf output/*
