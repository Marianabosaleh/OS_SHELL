CC = gcc
CFLAGS = -Wall -Werror -fpic
LDFLAGS = -shared

LIBRARY_A = libcodecA.so
LIBRARY_B = libcodecB.so

ENCODE = encode
DECODE = decode
CMP = cmp
COPY = copy

all: $(LIBRARY_A) $(LIBRARY_B) $(ENCODE) $(DECODE) $(CMP) $(COPY)

$(LIBRARY_A): codecA.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(LIBRARY_B): codecB.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(ENCODE): encode.c
	$(CC) $(CFLAGS) $^ -o $@ -ldl

$(DECODE): decode.c
	$(CC) $(CFLAGS) $^ -o $@ -ldl

$(CMP): cmp.c
	$(CC) $(CFLAGS) $^ -o $@

$(COPY): copy.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(LIBRARY_A) $(LIBRARY_B) $(ENCODE) $(DECODE) $(CMP) $(COPY)
