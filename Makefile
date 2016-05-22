srcfiles := $(shell find . -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(objects)
	g++ -o "bin/raytracer" $(objects)

run: all
	./bin/raytracer > image.ppm

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	g++ -MM $^>>./.depend;

clean:
	rm -f $(objects)

dist-clean: clean
	rm -f *~ .depend

include .depend
