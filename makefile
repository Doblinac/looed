CC = gcc
CFLAGS = -lncurses

all: vim_editor

vim_editor: main.c editor.c commands.c
	$(CC) main.c editor.c commands.c -o vim_editor $(CFLAGS)

clean:
	rm -f vim_editor