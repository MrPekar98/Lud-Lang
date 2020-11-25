CC = gcc
CFLAGS = -pedantic
LINK := util.c lexer.c parser.c type_checker.c
SRC = lud.c

lud: $(SRC) $(foreach link, $(LINK), $(link)) ;\
	$(CC) -o lud $(SRC) $(foreach link, $(LINK), $(link)) $(CFLAGS)

clean: ;\
	rm lud