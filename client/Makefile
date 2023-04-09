FLAGS = -std=c11 -Wall -Wextra -Wpedantic

NAME = uchat

GTK_LIBS =  -rdynamic `pkg-config --cflags --libs gtk+-3.0`
GTK_INC = `pkg-config --cflags gtk+-3.0`

all: install clean

uchat_server: install 

#-L ../local_lib/lib

$(NAME): install

install: clean
	mkdir -p obj
	clang $(FLAGS) $(GTK_INC) -c  src/*.c  src/reqBuilders/*.c  src/eventHandlers/*.c  src/respHandlers/*.c src/helpers/*.c -I ../lib/libmx/inc -I ../lib/openssl -I ../lib/cjson/inc -I ../lib/sqlite/inc
	mv *.o obj/
	clang $(FLAGS) obj/*.o -L ../lib/libmx -lmx  -lssl -lcrypto -lpthread -L ../lib/cjson -lcjson  -L ../lib/sqlite -lsqlite3  $(GTK_LIBS) -o $(NAME) 

uninstall: clean
	rm -rf obj
	rm -rf $(NAME)

clean: 
	rm -rf obj

reinstall: all