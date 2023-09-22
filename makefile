build:
	rm -rf ./bismuth
	g++ -std=c++20 $(options) main.cpp -o bismuth -lncurses

fresh:
	make build -j12
	make script=$(script) run

run:
	./bismuth $(script)

verbose:
	./bismuth -v $(script)

test:
	make build -j12
	make script=$(script) verbose

debug:
	make options=-g build
	lldb -o run bismuth verbose $(script)

deploy:
	make build
	make install

install:
	rm /usr/local/bin/bismuth
	cp bismuth /usr/local/bin/bismuth