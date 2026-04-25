build:
	clang++ -std=c++20 -fomit-frame-pointer -fnew-alignment 8 -O3 -shared -undefined dynamic_lookup $(shell simple2d --libs) -o draw.crystal draw.cpp