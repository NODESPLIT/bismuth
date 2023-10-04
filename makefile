assemble:
	python3 assemble.py

build:
	rm -rf ./bismuth
	make assemble
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

bench:
	/usr/bin/time -l -h -p ./bismuth scripts/bench/fib

tune:
	make build
	make bench