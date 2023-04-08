FLAGS = -std=c11 -Wall -Wextra -Wpedantic

NAME = uchat_server

all: install clean

uchat_server: install 

$(NAME): install

#-L ../local_lib/lib

install: clean
	mkdir -p obj
	clang $(FLAGS) -c src/*.c src/dbUtils/*.c  src/req_handlers/*.c -I ../lib/libmx/inc \
	               -I ../lib/openssl \
	               -I ../lib/cjson/inc \
	               -I ../lib/sqlite/inc
	mv *.o obj/
	clang $(FLAGS) obj/*.o -L ../lib/libmx -lmx -lssl -lcrypto -lpthread -L ../lib/cjson -lcjson -L ../lib/sqlite -lsqlite3  -ldl -o $(NAME)

uninstall: clean
	rm -rf obj
	rm -rf $(NAME)

clean: 
	rm -rf obj

reinstall: all
