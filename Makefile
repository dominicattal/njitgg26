CC = gcc
CFLAGS = -IC:/raylib/raylib/src -LC:/raylib/raylib/src -Wall -Wextra -Werror -Wfatal-errors -std=c11 -lraylib -lopengl32 -lgdi32 -lwinmm -Wno-unused-parameter -Wno-unused-function
SOURCES = main.c \
		  json.c \
		  game.c \
		  gui.c \
		  screen.c \
		  item.c \
		  raygui.c \
		  config.c \
		  character.c

OBJS_DEV = $(SOURCES:%.c=build/dev/%.o)

all: dev

dev: $(OBJS_DEV)
	@mkdir -p bin
	@$(CC) $(OBJS_DEV) $(CFLAGS) -D DEBUG_BUILD -g3 -Og -o bin/dev.exe

build/dev/%.o: %.c
	@mkdir -p $(shell dirname $@)
	@echo $<
	@$(CC) $(CFLAGS) -D DEBUG_BUILD -g3 -Og -c -o $@ $<

clean:
	rm -rf build bin
.PHONY: clean
