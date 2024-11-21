SDL2FLAGS=`pkg-config --cflags sdl2`
SDL2LIBS=`pkg-config --libs sdl2` -lsdl2_image -lsdl2_mixer -lsdl2_ttf

build_mac:
	gcc -std=c99 ./src/*.c -o ./bin/raycast \
	-I/opt/homebrew/opt/sdl2/include \
    -L/opt/homebrew/opt/sdl2/lib \
    -lSDL2;

build_linux:
	gcc -std=c99 ./src/*.c -o ./bin/raycast -lSDL2;

run:
	./bin/raycast;

clean:
	rm ./bin/raycast;