build:
	mkdir build;
	i686-w64-mingw32-gcc -Wall ./src/*.c -o build/doom  -lSDL2 -lSDL2_image -lm
	cp -r ./src/resources ./build/resources
run:
	./build/doom

clean:
	rm -r build;