CFLAGS=-Wall -Werror -g -fsanitize=address
TARGETS=expr_whizz ew_test
OBJS
HDRS= pipeline.h token.h tokenize
LIBS=-lasan -lm -lreadline 

all: $(TARGETS)

expr_whizz: $(OBJS) expr_whizz.o
	gcc $(LDFLAGS) $^ $(LIBS) -o $@

ew_test: $(OBJS) ew_test.o
	gcc $(LDFLAGS) $^ $(LIBS) -o $@

%.o: %.c $(HDRS)
	gcc -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(TARGETS)
