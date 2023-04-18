build:
	g++ -w -std=c++14 -Wfatal-errors \
	./src/*.cpp \
	-o paultris \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer;

clean:
	rm ./paultris;

run:
	./paultris;
