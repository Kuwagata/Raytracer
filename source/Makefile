OBJS = 	main.o     \
	model.o       \
	environment.o \
	object.o      \
	sceneobj.o    \
	getnum.o      \
	getimage.o    \
	list.o        \
	triple.o      \
	plane.o       \
	sphere.o      \
	ray.o         \
	image.o       \
	illuminate.o  \
	light.o       \
	polygon.o     \
	triangle.o    \
	rectangle.o   \
	polytest.o    \
	tplane.o      \
	pplane.o      \
	cylinder.o    \

CFLAGS = -Wall -g -O2

all: main

main: ${OBJS}
	gcc -o raytrace -g ${OBJS} -lm

${OBJS}: ray.h rayhdrs.h triple.h list.h Makefile

clean:
	rm -f raytrace *.o

test:
	./raytrace ../input/world_all2.txt ../output/output.ppm 800 1
