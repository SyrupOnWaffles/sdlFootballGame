build:
	mkdir build;
	gcc -Wall ./src/*.c -o build/game  -lSDL2 -lSDL2_image -lSDL2_mixer -lm
	cp -r ./src/resources ./build/resources
windows-build:
	mkdir build;
	i686-w64-mingw32-gcc -Wall ./src/*.c -o build/game -I"SDL2/include" -L"SDL2/lib"  -lSDL2 -lSDL2_image -lSDL2_mixer -lm -municode
	cp -r ./src/resources ./build/resources
	cp SDL2/bin/SDL2.dll ./build/SDL2.dll
	cp SDL2/bin/SDL2_image.dll ./build/SDL2_image.dll
run:
	./build/game

clean:
	rm -r build;