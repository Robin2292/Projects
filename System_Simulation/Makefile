CC=gcc

CFLAGS=-I. -Wall -g

SOURCES= ProcessPage.c MemoryFrame.c MemoryList.c ProcessQueue.c allocate.c

OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE=allocate

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ -lm 

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) 

