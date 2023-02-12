FLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

all: install 

install: 
	make -C ./lib/libmx
	make -C ./lib/cjson
	make -C ./lib/sqlite
	make -C ./server
	make -C ./client
	mv server/uchat_server .
	mv client/uchat .

uninstall: 
	make -C ./lib/libmx uninstall
	make -C ./lib/cjson uninstall
	make -C ./lib/sqlite uninstall
	make -C ./server uninstall
	make -C ./client uninstall
	rm -rf uchat_server
	rm -rf uchat
