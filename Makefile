CFLAGS=-Wall -Werror -g -fsanitize=address
TARGETS=plaidsh pipeline_test
OBJS=clist.o tokenize.o pipeline.o parse.o
HDRS=clist.h token.h tokenize.h pipeline.h parse.h author.sh
LIBS=-lasan -lm -lreadline 

all: $(TARGETS)

plaidsh: $(OBJS) plaidsh.o
	gcc $(LDFLAGS) $^ $(LIBS) -o $@

pipeline_test: $(OBJS) pipeline_test.o
	gcc $(LDFLAGS) $^ $(LIBS) -o $@
	
%.o: %.c $(HDRS)
	gcc -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(TARGETS)
