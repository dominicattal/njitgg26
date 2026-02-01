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

OBJS_REL = $(SOURCES:%.c=build/release/%.o)
OBJS_DEV = $(SOURCES:%.c=build/dev/%.o)

all: dev

dev: $(OBJS_DEV)
	@mkdir -p bin
	@$(CC) $(OBJS_DEV) $(CFLAGS) -D DEBUG_BUILD -g3 -Og -o bin/dev.exe

build/dev/%.o: %.c
	@mkdir -p $(shell dirname $@)
	@echo $<
	@$(CC) $(CFLAGS) -D DEBUG_BUILD -g3 -Og -c -o $@ $<

release: $(OBJS_REL)
	@echo $(SOURCES)
	@mkdir -p bin
	@mkdir -p bin/MasqueradeMacabre
	@echo $(OBJS_REL)
	@$(CC) $(OBJS_REL) -static $(CFLAGS) -mwindows -O2 -o bin/MasqueradeMacabre/masquerademacabre.exe
	@cp -r assets config bin/MasqueradeMacabre

build/release/%.o: %.c
	@echo $<
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -O2 -c -o $@ $<

clean:
	rm -rf build bin
.PHONY: clean
