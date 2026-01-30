all:
	gcc core_basic_window.c -I../raylib/raylib/src -L../raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
