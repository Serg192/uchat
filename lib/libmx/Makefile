.PHONY: all libmx uninstall clean reinstall

FLAGS = -Wall -Werror -Wextra -Wpedantic 

all: libmx

libmx:
	clang $(FLAGS) -c src/*.c
	mkdir -p  obj
	mv *.o obj
	ar -rcs libmx.a obj/*.o
	ranlib libmx.a

uninstall: clean
	rm -f libmx.a

clean:
	rm -rfd obj

reinstall: uninstall libmx
