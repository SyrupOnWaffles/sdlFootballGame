build:
	mkdir build;
	gcc -Wall ./src/*.c -o build/doom  -lSDL2 -lSDL2_image -lm
	cp -r ./src/resources ./build/resources
windows-build:
	mkdir build;
	i686-w64-mingw32-gcc -Wall ./src/*.c -o build/doom -I"SDL2/include" -L"SDL2/lib"  -lSDL2 -lSDL2_image -lm -municode
	cp -r ./src/resources ./build/resources
	cp SDL2/bin/SDL2.dll ./build/SDL2.dll
	cp SDL2/bin/SDL2_image.dll ./build/SDL2_image.dll
run:
	./build/doom

clean:
	rm -r build;