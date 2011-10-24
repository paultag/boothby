all:
	cd src/ && make
clean:
	cd src/ && make clean
	rm -rf bin/*
test: all
	echo "./color.sh" | ./bin/boothby 2>log
