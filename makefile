CC = gcc
CFLAGS = -lncursesw

all: vim_editor

vim_editor: main.c editor.c commands.c render.c clipboard.c editor_normal.c editor_insert.c utils.c scroll.c
	$(CC) main.c editor.c editor_normal.c editor_insert.c utils.c commands.c render.c clipboard.c scroll.c -o vim_editor $(CFLAGS)

clean:
	rm -f vim_editor