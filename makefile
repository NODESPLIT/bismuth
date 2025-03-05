assemble:
	python3 assemble.py

build:
	rm -rf ./bismuth
	make assemble
	clang++ -std=c++20 -O0 $(options) -Wall main.cpp -o bismuth -llinenoise

optimal:
	rm -rf ./bismuth
	make assemble
	clang++ -std=c++20 -fomit-frame-pointer -fnew-alignment 8 -O3 $(options) -Wall main.cpp -o bismuth -llinenoise /opt/homebrew/Cellar/gperftools/2.16/lib/libtcmalloc_and_profiler.a

superoptimal:
	rm -rf ./bismuth
	make assemble
	/Users/anom.li/souper/build/sclang++ -std=c++20 \
		-fomit-frame-pointer -fnew-alignment 8 $(options) -Wall \
		main.cpp -o bismuth \
		-llinenoise /opt/homebrew/Cellar/gperftools/2.16/lib/libtcmalloc_and_profiler.a

fresh:
	make optimal -j10
	make script=$(script) run

run:
	./bismuth $(script)

verbose:
	./bismuth -v $(script)

test:
	make build -j10
	make script=$(script) verbose

debug:
	make options=-g build
	lldb -o run bismuth verbose $(script)

deploy:
	make optimal
	make install

install:
	rm /usr/local/bin/bismuth
	cp bismuth /usr/local/bin/bismuth

bench:
	/usr/bin/time -l -h -p ./bismuth scripts/bench/fib

tune:
	make optimal
	make bench