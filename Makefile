all:
	g++ -std=c++17 -I ./header -I ./scripts/header -I ./config -o ./pkRacer ./source/*.cpp ./scripts/source/*.cpp ./source/img/*.cpp ./source/3D/*.cpp -L"lib" -Wall -lSDL2main -lSDL2 -lGL -lGLU -lGLEW -lSDL2_ttf -pthread
	./pkRacer
compile:
	g++ -std=c++17 -I ./header -I ./scripts/header -I ./config -o ./pkRacer ./source/*.cpp ./scripts/source/*.cpp ./source/img/*.cpp ./source/3D/*.cpp -L"lib" -Wall -lSDL2main -lSDL2 -lGL -lGLU -lGLEW -lSDL2_ttf -pthread
run:
	./pkRacer
push:
	git add ./header ./source ./scripts/header ./scripts/source ./config ./fonts ./img ./Makefile ./README.md
	git commit -a -m "Readme Update"
	git push origin main
lnct:
	lnct -r -e ./fonts -e ./header/SDL -e ./img -e ./.git -e ./.vscode