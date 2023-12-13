all: build

build:
	gcc cbqn-80.c $(shell pkg-config --cflags --libs sdl2) -lcbqn -o cbqn-80
