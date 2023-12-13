all: build

build:
	gcc cbqn-80.c $(shell pkg-config --cflags --libs sdl2) \
	  $(shell pkg-config --cflags --libs SDL2_image) \
	  -lcbqn -o cbqn-80
