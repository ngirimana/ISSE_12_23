CFLAGS=-Wall -Werror -g -fsanitize=address
TARGETS=tokenize_test plaidsh
OBJS=plist.o tokenize.o
HDRS=plist.h token.h tokenize.h 
LIBS=-lasan -lm -lreadline 

all: $(TARGETS)

plaidsh: $(OBJS) plaidsh.o
	gcc $(LDFLAGS) $^ $(LIBS) -o $@

tokenize_test: $(OBJS) tokenize_test.o
	gcc $(LDFLAGS) $^ $(LIBS) -o $@

%.o: %.c $(HDRS)
	gcc -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(TARGETS)
