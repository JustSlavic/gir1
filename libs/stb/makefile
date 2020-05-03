PROJECT=stb
CXX=g++
CXX_STANDARD=c++14


.PHONY: clean


all: build/stb_image.o
	@mkdir -p bin
	ar rcvs bin/lib$(PROJECT).a build/stb_image.o

build/stb_image.o: stb_image.h stb_image.cpp
	@mkdir -p build
	$(CXX) stb_image.cpp -c -o build/stb_image.o -std=$(CXX_STANDARD)


clean:
	find build -type f -name '*.o' -delete
	rm -fv bin/lib$(PROJECT).a
