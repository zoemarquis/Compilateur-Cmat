
CC=gcc
CFLAGS=-g -fcommon

all: cmat

cmat: cmat.c EXPR.tab.o lex.yy.o matrix.o pile.o hashtab.o symbtab.o generation.o
	${CC} -o $@ $^

EXPR.tab.o: EXPR.tab.c error.h matrix.h pile.h hashtab.h symbtab.h generation.h

EXPR.tab.c: EXPR.y
	bison -d $<

lex.yy.o: lex.yy.c

lex.yy.c: EXPR.lex EXPR.tab.c
	flex EXPR.lex

matrix.o: CFLAGS+=-Wall -Wextra
matrix.o: matrix.c matrix.h error.h
	${CC} ${CFLAGS} -c $< -o $@


pile.o: CFLAGS+=-Wall -Wextra
pile.o: pile.c pile.h
	${CC} ${CFLAGS} -c $< -o $@

# variable.o: CFLAGS+=-Wall -Wextra
# variable.o: variable.c variable.h matrix.h symbtab.h error.h
# 	${CC} ${CFLAGS} -c $< -o $@

hashtab.o: CFLAGS+=-Wall -Wextra
hashtab.o: hashtab.c hashtab.h #matrix.h error.h
	${CC} ${CFLAGS} -c $< -o $@

symbtab.o: CFLAGS+=-Wall -Wextra
symbtab.o: symbtab.c symbtab.h matrix.h error.h
	${CC} ${CFLAGS} -c $< -o $@

generation.o: CFLAGS+=-Wall -Wextra 
generation.o: generation.c generation.h symbtab.h error.h pile.h
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f cmat *.o EXPR.tab.c EXPR.tab.h lex.yy.c *~
	rm -rf output/*
