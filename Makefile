CC = g++
CCFLAGS= -O2 -fpermissive -std=c++14
LEX = flex
LFLAGS= -8     
YACC= bison 
YFLAGS= -d -t -y

lang: y.tab.o lex.yy.o 
	${CC} ${CCFLAGS} lex.yy.o y.tab.o -o lang 

y.tab.o: lang.y *.hpp
	${YACC} ${YFLAGS} lang.y
	${CC} ${CCFLAGS} y.tab.c -c 

lex.yy.o: lang.l *.hpp
	${LEX} $(LFLAGS) lang.l
	${CC} ${CCFLAGS} lex.yy.c -c 

clean:
	rm -f lex.yy.* y.tab.* *.o lang
