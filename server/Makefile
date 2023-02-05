FLAGS = -std=c11 -Wall -Wextra -Wpedantic

NAME = uchat_server

all: install clean

uchat_server: install 

$(NAME): install

install: clean
	mkdir -p obj
	clang $(FLAGS) -c src/*.c -I ../lib/libmx/inc -I ../lib/openssl
	mv *.o obj/
	clang $(FLAGS) obj/*.o -L ../lib/libmx -lmx -lssl -lcrypto -o $(NAME)

uninstall: clean
	rm -rf obj
	rm -rf $(NAME)

clean: 
	rm -rf obj

reinstall: all
