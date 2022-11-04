all: build

build: PPM_Image_Editor.c
	gcc -g -Wall -std=c99 -o PPM_Image_Editor PPM_Image_Editor.c

clean: build
	rm PPM_Image_Editor