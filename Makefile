all: compile run
compile:
	g++ *.cpp -o main -I ./deps/include/ -L ./deps/lib/ -lraylib -lGL -lm -lpthread -ldl -lrt
run:
	./main
webcompile:
	emcc -std=c++17 -o ./build/index.html *.cpp -Os -Wall -I ./include/ -L ./lib/ -lraylib -s USE_GLFW=3 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH -DPLATFORM_WEB --preload-file assets
